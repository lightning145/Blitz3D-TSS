#ifndef GXRUNTIME_H
#define GXRUNTIME_H

#include <Windows.h>
#include <string>
#include <vector>
#include <intrin.h>

#include "gxfilesystem.h"
#include "gxtimer.h"

#include "../debugger/debugger.h"

class gxRuntime {
	/***** INTERNAL INTERFACE *****/
public:
	HWND hwnd;
	HINSTANCE hinst;

	HDC hDC;
	HGLRC hRC;

	gxFileSystem* fileSystem;

	float scale_x = .0f, scale_y = .0f;

	LRESULT windowProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

	void InitWindow(int width, int height);
	void FreeWindow();
	void SwapBackBuffer();

	int GetWidth()
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		return rect.right - rect.left;
	}

	int GetHeight()
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		return rect.bottom - rect.top;
	}

private:
	gxRuntime(HINSTANCE hinst, const std::string& cmd_line, HWND hwnd);
	~gxRuntime();

	void paint();
	void suspend();
	void forceSuspend();
	void resume();
	void forceResume();

	RECT t_rect;
	int t_style;
	std::string cmd_line;
	bool pointer_visible;
	std::string app_title;
	std::string app_close;

	bool enum_all;
	int use_di;  

	void resetInput(); 
	void pauseAudio();  
	void resumeAudio(); 

	/***** APP INTERFACE *****/
public:
	static gxRuntime* openRuntime(HINSTANCE hinst, const std::string& cmd_line, Debugger* debugger);
	static void closeRuntime(gxRuntime* runtime);

	void asyncStop();
	void asyncRun();
	void asyncEnd();


	/***** GX INTERFACE *****/
public:
	//return true if program should continue, or false for quit.
	bool idle();
	bool delay(int ms);

	bool execute(const std::string& cmd);
	void setTitle(const std::string& title, const std::string& close);
	int  getMilliSecs();
	void setPointerVisible(bool vis);

	std::string commandLine();

	std::string systemProperty(const std::string& t);

	void debugStop();
	bool debugStmt(int pos, const char* file);
	void debugEnter(void* frame, void* env, const char* func);
	void debugLeave();
	void debugInfo(const char* t);
	void debugError(const char* t);
	void debugLog(const char* t);

	bool focus();
	int desktopWidth();
	int desktopHeight();

	gxFileSystem* openFileSystem(int flags);
	void closeFileSystem(gxFileSystem* filesys);

	gxTimer* createTimer(int hertz);
	void freeTimer(gxTimer* timer);

	void calculateDPI();

	void enableDirectInput(bool use);
	int  directInputEnabled() { return use_di; }

	int callDll(const std::string& dll, const std::string& func, const void* in, int in_sz, void* out, int out_sz);

	OSVERSIONINFO osinfo;
	MEMORYSTATUSEX statex;
	DEVMODE devmode;

	int getMemoryLoad();
	int getTotalPhys();
	int getAvailPhys();
	int getTotalVirtual();
	int getAvailVirtual();
};

#endif