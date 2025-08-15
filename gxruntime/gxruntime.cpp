#include "std.h"
#include "gxruntime.h"
#include "../gxruntime/gxutf8.h"

static const int static_ws = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
static const int scaled_ws = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

static std::string app_title;
static std::string app_close;
static gxRuntime* runtime;
static bool busy, suspended;
static volatile bool run_flag;

typedef int(_stdcall* LibFunc)(const void* in, int in_sz, void* out, int out_sz);

struct gxDll {
	HINSTANCE hinst;
	std::map<std::string, LibFunc> funcs;
};

static std::map<std::string, gxDll*> libs;

static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

static std::set<gxTimer*> timers;

enum {
	WM_STOP = WM_APP + 1, WM_RUN, WM_END
};

////////////////////
// STATIC STARTUP //
////////////////////
gxRuntime* gxRuntime::openRuntime(HINSTANCE hinst, const std::string& cmd_line, Debugger* d) {
	if (runtime) return 0;

	//create debugger
	Debugger* debugger = d;

	//create WNDCLASS
	WNDCLASS wndclass;
	memset(&wndclass, 0, sizeof(wndclass));
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.lpfnWndProc = ::windowProc;
	wndclass.hInstance = hinst;
	wndclass.lpszClassName = "Blitz Runtime Class";
	wndclass.hCursor = (HCURSOR)LoadCursor(0, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	RegisterClass(&wndclass);

	busy = suspended = false;
	run_flag = true;

	const char* app_t = " ";
	int ws = WS_CAPTION, ws_ex = 0;

	HWND hwnd = CreateWindowEx(ws_ex, "Blitz Runtime Class", app_t, ws, 0, 0, 0, 0, 0, 0, 0, 0);
	UpdateWindow(hwnd);

	runtime = new gxRuntime(hinst, cmd_line, hwnd);
	return runtime;
}

void gxRuntime::closeRuntime(gxRuntime* r) {
	if (!runtime || runtime != r) return;

	std::map<std::string, gxDll*>::const_iterator it;
	for (it = libs.begin(); it != libs.end(); ++it) {
		FreeLibrary(it->second->hinst);
	}
	libs.clear();

	delete runtime;
	runtime = 0;
}

//////////////////////////
// RUNTIME CONSTRUCTION //
//////////////////////////
gxRuntime::gxRuntime(HINSTANCE hi, const std::string& cl, HWND hw) :
	hinst(hi), cmd_line(cl), hwnd(hw), enum_all(false),
	pointer_visible(true), fileSystem(0), use_di(false) {

	memset(&osinfo, 0, sizeof(osinfo));
	osinfo.dwOSVersionInfoSize = sizeof(osinfo);

	HMODULE osinfodll = LoadLibraryA("ntdll.dll");
	if (osinfodll) {
		typedef void (WINAPI* RtlGetVersionFunc)(OSVERSIONINFO*);
		RtlGetVersionFunc RtlGetVersion = (RtlGetVersionFunc)GetProcAddress(osinfodll, "RtlGetVersion");
		if (RtlGetVersion) RtlGetVersion(&osinfo);
		FreeLibrary(osinfodll);
	}

	memset(&statex, 0, sizeof(statex));
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	memset(&devmode, 0, sizeof(devmode));
	devmode.dmSize = sizeof(devmode);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);

	TIMECAPS tc;
	timeGetDevCaps(&tc, sizeof(tc));
	timeBeginPeriod(tc.wPeriodMin);
}

gxRuntime::~gxRuntime() {
	
	while (timers.size()) freeTimer(*timers.begin());

	if (fileSystem) closeFileSystem(fileSystem);

	TIMECAPS tc;
	timeGetDevCaps(&tc, sizeof(tc));
	timeEndPeriod(tc.wPeriodMin);

	DestroyWindow(hwnd);
	UnregisterClass("Blitz Runtime Class", hinst);
}

void gxRuntime::InitWindow(int width, int height)
{

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int x = (screenWidth - width) / 2;
	int y = (screenHeight - height) / 2;

	LONG style = scaled_ws;
	SetWindowLong(hwnd, GWL_STYLE, style);

	RECT windowRect = { 0, 0, width, height };
	AdjustWindowRect(&windowRect, style, FALSE);
	int adjustedWidth = windowRect.right - windowRect.left;
	int adjustedHeight = windowRect.bottom - windowRect.top;

	SetWindowPos(
		hwnd,
		HWND_TOP,
		x, y,
		adjustedWidth, adjustedHeight,
		SWP_SHOWWINDOW | SWP_NOZORDER
	);

	LONG ss = GetWindowLong(hwnd, GWL_STYLE);
	ss &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
	SetWindowLong(hwnd, GWL_STYLE, ss);

	UpdateWindow(hwnd);
	
}

void gxRuntime::pauseAudio() {
	
}

void gxRuntime::resumeAudio() {
	
}

void gxRuntime::resetInput() {

}

/////////////
// SUSPEND //
/////////////
void gxRuntime::suspend() {
	busy = true;
	pauseAudio();
	resetInput();
	suspended = true;
	busy = false;

	ShowCursor(1);
	if (Debugger* debugger = nullptr) debugger->debugStop();
}

////////////
// RESUME //
////////////
void gxRuntime::resume() {
	ShowCursor(0);
	busy = true;
	resetInput();
	resumeAudio();
	suspended = false;
	busy = false;

	if (Debugger* debugger = nullptr) debugger->debugRun();
}

///////////////////
// FORCE SUSPEND //
///////////////////
void gxRuntime::forceSuspend() {
	SetForegroundWindow(GetDesktopWindow());
	ShowWindow(GetDesktopWindow(), SW_SHOW);
	suspend();
}

//////////////////
// FORCE RESUME //
//////////////////
void gxRuntime::forceResume() {
	SetForegroundWindow(hwnd);
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	resume();
}

///////////
// PAINT //
///////////
void gxRuntime::paint() {
	HDC hdc = GetDC(hwnd);
	RECT rect;
	GetClientRect(hwnd, &rect);
	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
	ReleaseDC(hwnd, hdc);
}

/////////////////
// WINDOW PROC //
/////////////////
LRESULT gxRuntime::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (busy) {
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	PAINTSTRUCT ps;

	switch (msg) {
	case WM_PAINT:
		BeginPaint(hwnd, &ps);
		//paint();
		EndPaint(hwnd, &ps);
		return 0;
	case WM_ERASEBKGND:
		return 1;
	case WM_CLOSE:
		if (app_close.size()) {
			int n = MessageBox(hwnd, app_close.c_str(), app_title.c_str(), MB_OKCANCEL | MB_ICONWARNING | MB_SETFOREGROUND | MB_TOPMOST);
			if (n != IDOK) return 0;
		}
		asyncEnd();
		return 0;
	case WM_SETCURSOR:
		if (!suspended && !pointer_visible) {
			SetCursor(0);
			return 1;
		}
		break;
	case WM_ACTIVATEAPP:
		if (wparam) {
			if (suspended) resume();
		}
		else {
			if (!suspended) suspend();
		}
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (runtime) return runtime->windowProc(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//////////////////////////////
// 外部控制消息处理 //
//////////////////////////////
void gxRuntime::asyncStop() {
	PostMessage(hwnd, WM_STOP, 0, 0);
}

void gxRuntime::asyncRun() {
	PostMessage(hwnd, WM_RUN, 0, 0);
}

void gxRuntime::asyncEnd() {
	PostMessage(hwnd, WM_END, 0, 0);
}

//////////
// IDLE //
//////////
bool gxRuntime::idle() {
	for (;;) {
		MSG msg;
		BOOL success = 0;
		if (suspended && run_flag) {
			success = GetMessageW(&msg, 0, 0, 0);
		}
		else {
			if (!PeekMessageW(&msg, 0, 0, 0, PM_REMOVE)) return run_flag;
		}
		switch (msg.message) {
		case WM_STOP:
			if (!suspended) forceSuspend();
			break;
		case WM_RUN:
			if (suspended) forceResume();
			break;
		case WM_END:
			run_flag = false;
			break;
		default:
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	return run_flag;
}

///////////
// DELAY //
///////////
bool gxRuntime::delay(int ms) {
	int t = timeGetTime() + ms;
	for (;;) {
		if (!idle()) return false;
		int d = t - timeGetTime();
		if (d <= 0) return true;
		if (d > 100) d = 100;
		Sleep(d);
	}
}

///////////////
// DEBUG  //
///////////////
bool gxRuntime::debugStmt(int pos, const char* file) {
	return false;  
}

void gxRuntime::debugStop() {
	if (!suspended) forceSuspend();
}

void gxRuntime::debugEnter(void* frame, void* env, const char* func) {

}

void gxRuntime::debugLeave() {

}

void gxRuntime::debugError(const char* t) {
	asyncEnd();
	if (!suspended) forceSuspend();
	MessageBoxA(hwnd, t, "Error", MB_OK | MB_ICONERROR);
}

void gxRuntime::debugInfo(const char* t) {
	asyncEnd();
	if (!suspended) forceSuspend();
	MessageBoxA(hwnd, t, "Info", MB_OK | MB_ICONINFORMATION);
}

void gxRuntime::debugLog(const char* t) {
	OutputDebugStringA(t);
}

std::string gxRuntime::commandLine() {
	return cmd_line;
}

bool gxRuntime::execute(const std::string& cmd_line) {
	if (!cmd_line.size()) return false;

	std::string cmd = cmd_line, params;
	while (cmd.size() && cmd[0] == ' ') cmd = cmd.substr(1);
	if (cmd.find('\"') == 0) {
		int n = cmd.find('\"', 1);
		if (n != std::string::npos) {
			params = cmd.substr(n + 1);
			cmd = cmd.substr(1, n - 1);
		}
	}
	else {
		int n = cmd.find(' ');
		if (n != std::string::npos) {
			params = cmd.substr(n + 1);
			cmd = cmd.substr(0, n);
		}
	}
	while (params.size() && params[0] == ' ') params = params.substr(1);
	while (params.size() && params[params.size() - 1] == ' ') params = params.substr(0, params.size() - 1);

	SetForegroundWindow(GetDesktopWindow());
	return (int)ShellExecute(GetDesktopWindow(), 0, cmd.c_str(), params.size() ? params.c_str() : 0, 0, SW_SHOW) > 32;
}

void gxRuntime::setTitle(const std::string& t, const std::string& e) {
	app_title = t;
	app_close = e;
	SetWindowTextW(hwnd, UTF8::convertToUtf16(app_title).c_str());
}

int gxRuntime::getMilliSecs() {
	return timeGetTime() & 0x7FFFFFFF;
}

int gxRuntime::getMemoryLoad() {
	GlobalMemoryStatusEx(&statex);
	return statex.dwMemoryLoad;
}

int gxRuntime::getTotalPhys() {
	return statex.ullTotalPhys / 1024;
}

int gxRuntime::getAvailPhys() {
	return statex.ullAvailPhys / 1024;
}

int gxRuntime::getTotalVirtual() {
	return statex.ullTotalVirtual / 1024;
}

int gxRuntime::getAvailVirtual() {
	return statex.ullAvailVirtual / 1024;
}

void gxRuntime::setPointerVisible(bool vis) {
	if (pointer_visible == vis) return;
	pointer_visible = vis;
	POINT pt;
	GetCursorPos(&pt);
	SetCursorPos(pt.x, pt.y); 
}

gxFileSystem* gxRuntime::openFileSystem(int flags) {
	if (fileSystem) return 0;
	fileSystem = new gxFileSystem();
	return fileSystem;
}

void gxRuntime::closeFileSystem(gxFileSystem* f) {
	if (!fileSystem || fileSystem != f) return;
	delete fileSystem;
	fileSystem = 0;
}

/////////////////
// Timer //
/////////////////
gxTimer* gxRuntime::createTimer(int hertz) {
	gxTimer* t = new gxTimer(this, hertz);
	timers.insert(t);
	return t;
}

void gxRuntime::freeTimer(gxTimer* t) {
	if (!timers.count(t)) return;
	timers.erase(t);
	delete t;
}

/////////////////
// System //
/////////////////
std::string gxRuntime::systemProperty(const std::string& p) {
	char buff[MAX_PATH + 1];
	std::string t = tolower(p);

	if (t == "os") {
		switch (osinfo.dwMajorVersion) {
		case 6:
			switch (osinfo.dwMinorVersion) {
			case 0: return "Windows Vista";
			case 1: return "Windows 7";
			case 2: return "Windows 8";
			case 3: return "Windows 8.1";
			}
			break;
		case 10:
			if (osinfo.dwBuildNumber >= 22000) return "Windows 11";
			return "Windows 10";
		}
	}
	else if (t == "cpuname") {
		std::string cpuBrand;
		uint32_t regs[4];
		__cpuid((int*)regs, 0x80000000);
		int numberOfExtendedFlags = regs[0];

		if (numberOfExtendedFlags >= 0x80000004) {
			__cpuid((int*)regs, 0x80000002);
			cpuBrand += std::string((const char*)&regs[0], 4);
			cpuBrand += std::string((const char*)&regs[1], 4);
			cpuBrand += std::string((const char*)&regs[2], 4);
			cpuBrand += std::string((const char*)&regs[3], 4);

			__cpuid((int*)regs, 0x80000003);
			cpuBrand += std::string((const char*)&regs[0], 4);
			cpuBrand += std::string((const char*)&regs[1], 4);
			cpuBrand += std::string((const char*)&regs[2], 4);
			cpuBrand += std::string((const char*)&regs[3], 4);

			__cpuid((int*)regs, 0x80000004);
			cpuBrand += std::string((const char*)&regs[0], 4);
			cpuBrand += std::string((const char*)&regs[1], 4);
			cpuBrand += std::string((const char*)&regs[2], 4);
			cpuBrand += std::string((const char*)&regs[3], 4);
		}
		else cpuBrand = getenv("PROCESSOR_IDENTIFIER");
		return cpuBrand;
	}
	else if (t == "cpuarch") {
		SYSTEM_INFO systemInfo;
		GetNativeSystemInfo(&systemInfo);
		switch (systemInfo.wProcessorArchitecture) {
		case PROCESSOR_ARCHITECTURE_AMD64: return "AMD64";
		case PROCESSOR_ARCHITECTURE_INTEL: return "x86";
		case PROCESSOR_ARCHITECTURE_ARM: return "ARM32";
		case PROCESSOR_ARCHITECTURE_ARM64: return "ARM64";
		default: return "Unknown";
		}
	}
	else if (t == "osbuild") {
		return std::to_string(osinfo.dwBuildNumber);
	}
	else if (t == "appdir") {
		if (GetModuleFileName(0, buff, MAX_PATH)) {
			std::string t = buff;
			int n = t.find_last_of('\\');
			if (n != std::string::npos) t = t.substr(0, n);
			return t + "\\";
		}
	}
	else if (t == "appfile") {
		if (GetModuleFileName(0, buff, MAX_PATH)) return buff;
	}
	else if (t == "windowsdir") {
		if (GetWindowsDirectory(buff, MAX_PATH)) return std::string(buff) + "\\";
	}
	else if (t == "systemdir") {
		if (GetSystemDirectory(buff, MAX_PATH)) return std::string(buff) + "\\";
	}
	else if (t == "tempdir") {
		if (GetTempPath(MAX_PATH, buff)) return std::string(buff) + "\\";
	}
	return "";
}

/////////////////
// DPI //
/////////////////
void gxRuntime::calculateDPI() {
	if ((scale_x == .0f) && (scale_y == .0f)) {
		HDC hdc = GetDC(GetDesktopWindow());
		scale_x = GetDeviceCaps(hdc, LOGPIXELSX) / 96.0f;
		scale_y = GetDeviceCaps(hdc, LOGPIXELSY) / 96.0f;
		ReleaseDC(GetDesktopWindow(), hdc);
	}
}

void gxRuntime::enableDirectInput(bool enable) {
	use_di = enable;
}

/////////////////
//Call DLL //
/////////////////
int gxRuntime::callDll(const std::string& dll, const std::string& func, const void* in, int in_sz, void* out, int out_sz) {
	std::map<std::string, gxDll*>::const_iterator lib_it = libs.find(dll);
	if (lib_it == libs.end()) {
		HINSTANCE h = LoadLibrary(dll.c_str());
		if (!h) return 0;
		gxDll* t = new gxDll;
		t->hinst = h;
		lib_it = libs.insert(make_pair(dll, t)).first;
	}

	gxDll* t = lib_it->second;
	std::map<std::string, LibFunc>::const_iterator fun_it = t->funcs.find(func);
	if (fun_it == t->funcs.end()) {
		LibFunc f = (LibFunc)GetProcAddress(t->hinst, func.c_str());
		if (!f) return 0;
		fun_it = t->funcs.insert(make_pair(func, f)).first;
	}

	static void* save_esp;
	_asm { mov[save_esp], esp };
	int n = fun_it->second(in, in_sz, out, out_sz);
	_asm { mov esp, [save_esp] };
	return n;
}

bool gxRuntime::focus() {
	return GetFocus() == hwnd;
}

int gxRuntime::desktopWidth() {
	return devmode.dmPelsWidth;
}

int gxRuntime::desktopHeight() {
	return devmode.dmPelsHeight;
}