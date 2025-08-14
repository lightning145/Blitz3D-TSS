#include "std.h"
#include "gxgraphics.h"
#include "gxruntime.h"
#include "../gxruntime/gxutf8.h"

extern gxRuntime* gx_runtime;
static Debugger* debugger;

gxGraphics::gxGraphics(gxRuntime* rt, IDirectDraw7* dd, IDirectDrawSurface7* fs, IDirectDrawSurface7* bs, bool d3d) :
	runtime(rt), dirDraw(dd), gfx_lost(false) {
	dirDraw->QueryInterface(IID_IDirectDraw, (void**)&ds_dirDraw);

	front_canvas = new gxCanvas(this, fs, 0);
	back_canvas = new gxCanvas(this, bs, 0);

	front_canvas->cls();
	back_canvas->cls();

	FT_Init_FreeType(&ftLibrary);

	HMODULE ntdllModule = GetModuleHandleW(L"ntdll.dll");
	running_on_wine = ntdllModule && GetProcAddress(ntdllModule, "wine_get_version");

	def_font = running_on_wine ? nullptr : this->loadFont(UTF8::getSystemFontFile("Courier"), 12);

	front_canvas->setFont(def_font);
	back_canvas->setFont(def_font);

	memset(&primFmt, 0, sizeof(primFmt));
	primFmt.dwSize = sizeof(primFmt);
	fs->GetPixelFormat(&primFmt);

	getTotalVidmem();

	//are we fullscreen?
	_gamma = 0;
	if (fs != bs) {
		if (fs->QueryInterface(IID_IDirectDrawGammaControl, (void**)&_gamma) >= 0) {
			if (_gamma->GetGammaRamp(0, &_gammaRamp) < 0) _gamma = 0;
		}
	}
	if (!_gamma) {
		for (int k = 0; k < 256; ++k) _gammaRamp.red[k] = _gammaRamp.blue[k] = _gammaRamp.green[k] = k;
	}
}

gxGraphics::~gxGraphics() {
	if (_gamma) _gamma->Release();
	while (movie_set.size()) closeMovie(*movie_set.begin());
	while (font_set.size()) freeFont(*font_set.begin());
	while (canvas_set.size()) freeCanvas(*canvas_set.begin());
	/*
	std::set<std::set<std::any>*>::iterator custom_set_it;
	for (custom_set_it = custom_set.begin(); custom_set_it != custom_set.end(); ++custom_set_it) {
		while ((*custom_set_it)->size()) {
			(*custom_set_it)->erase((*custom_set_it)->begin());
			delete* custom_set_it;
		}
	}
	*/

	std::set<std::string>::iterator it;
	for (it = font_res.begin(); it != font_res.end(); ++it) RemoveFontResource((*it).c_str());
	font_res.clear();

	delete back_canvas;
	delete front_canvas;

	FT_Done_FreeType(ftLibrary);

	ds_dirDraw->Release();

	dirDraw->RestoreDisplayMode();
	dirDraw->Release();
}

void gxGraphics::setGamma(int r, int g, int b, float dr, float dg, float db) {
	_gammaRamp.red[r & 255] = dr * 257.0f;
	_gammaRamp.green[g & 255] = dg * 257.0f;
	_gammaRamp.blue[b & 255] = db * 257.0f;
}

void gxGraphics::updateGamma(bool calibrate) {
	if (!_gamma) return;
	_gamma->SetGammaRamp(calibrate ? DDSGR_CALIBRATE : 0, &_gammaRamp);
}

void gxGraphics::getGamma(int r, int g, int b, float* dr, float* dg, float* db) {
	*dr = _gammaRamp.red[r & 255] / 257.0f;
	*dg = _gammaRamp.green[g & 255] / 257.0f;
	*db = _gammaRamp.blue[b & 255] / 257.0f;
}

bool gxGraphics::restore() {

	while (dirDraw->TestCooperativeLevel() != DD_OK) {

		if (dirDraw->TestCooperativeLevel() == DDERR_WRONGMODE) return false;

		Sleep(100);
	}

	if (back_canvas->getSurface()->IsLost() == DD_OK) return true;

	dirDraw->RestoreAllSurfaces();

	//restore all canvases
	std::set<gxCanvas*>::iterator it;
	for (it = canvas_set.begin(); it != canvas_set.end(); ++it) {
		(*it)->restore();
	}

	return true;
}

gxCanvas* gxGraphics::getFrontCanvas()const {
	return front_canvas;
}

gxCanvas* gxGraphics::getBackCanvas()const {
	return back_canvas;
}

gxFont* gxGraphics::getDefaultFont()const {
	return def_font;
}

void gxGraphics::vwait() {
	dirDraw->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
}

void gxGraphics::flip(bool v) {
	runtime->flip(v);
}

void gxGraphics::copy(gxCanvas* dest, int dx, int dy, int dw, int dh, gxCanvas* src, int sx, int sy, int sw, int sh) {
	RECT r = { dx,dy,dx + dw,dy + dh };
	ddUtil::copy(dest->getSurface(), dx, dy, dw, dh, src->getSurface(), sx, sy, sw, sh);
	dest->damage(r);
}

int gxGraphics::getScanLine()const {
	DWORD t = 0;
	dirDraw->GetScanLine(&t);
	return t;
}

int gxGraphics::getTotalVidmem()const {
	DDCAPS caps = { sizeof(caps) };
	dirDraw->GetCaps(&caps, 0);
	return caps.dwVidMemTotal / 1024;
}

int gxGraphics::getAvailVidmem()const {
	DDCAPS caps = { sizeof(caps) };
	dirDraw->GetCaps(&caps, 0);
	return caps.dwVidMemFree / 1024;
}

gxMovie* gxGraphics::openMovie(const std::string& file, int flags) {

	IAMMultiMediaStream* iam_stream;

	if (CoCreateInstance(
		CLSID_AMMultiMediaStream, NULL, CLSCTX_INPROC_SERVER,
		IID_IAMMultiMediaStream, (void**)&iam_stream) == S_OK) {

		if (iam_stream->Initialize(STREAMTYPE_READ, AMMSF_NOGRAPHTHREAD, NULL) == S_OK) {

			if (iam_stream->AddMediaStream(ds_dirDraw, &MSPID_PrimaryVideo, 0, 0) == S_OK) {

				iam_stream->AddMediaStream(NULL, &MSPID_PrimaryAudio, AMMSF_ADDDEFAULTRENDERER, NULL);

				WCHAR* path = new WCHAR[file.size() + 1];
				MultiByteToWideChar(CP_ACP, 0, file.c_str(), -1, path, sizeof(WCHAR) * (file.size() + 1));
				int n = iam_stream->OpenFile(path, 0);
				delete path;

				if (n == S_OK) {
					gxMovie* movie = new gxMovie(this, iam_stream);
					movie->filename = file;
					movie_set.insert(movie);
					return movie;
				}
			}
		}
		iam_stream->Release();
	}
	return 0;
}

gxMovie* gxGraphics::verifyMovie(gxMovie* m) {
	return movie_set.count(m) ? m : 0;
}

void gxGraphics::closeMovie(gxMovie* m) {
	if (movie_set.erase(m)) delete m;
}

gxCanvas* gxGraphics::createCanvas(int w, int h, int flags) {
	ddSurf* s = ddUtil::createSurface(w, h, flags, this);
	if (!s) return 0;
	gxCanvas* c = new gxCanvas(this, s, flags);
	canvas_set.insert(c);
	c->cls();
	if (debugger) printf("Created new gxCanvas instance.");
	return c;
}

gxCanvas* gxGraphics::loadCanvas(const std::string& f, int flags) {
	ddSurf* s = ddUtil::loadSurface(f, flags, this);
	if (!s) return 0;
	gxCanvas* c = new gxCanvas(this, s, flags);
	canvas_set.insert(c);
	return c;
}

gxCanvas* gxGraphics::verifyCanvas(gxCanvas* c) {
	return canvas_set.count(c) || c == front_canvas || c == back_canvas ? c : 0;
}

void gxGraphics::freeCanvas(gxCanvas* c) {
	if (canvas_set.erase(c)) delete c;
}

int gxGraphics::getWidth()const {
	return front_canvas->getWidth();
}

int gxGraphics::getHeight()const {
	return front_canvas->getHeight();
}

int gxGraphics::getDepth()const {
	return front_canvas->getDepth();
}

gxFont* gxGraphics::loadFont(std::string f, int height, bool bold, bool italic, bool underlined) {
	std::string t;
	int n = f.find('.');
	if (n == std::string::npos) {
		t = fullfilename(f);
		if (!font_res.count(t) && AddFontResource(t.c_str())) font_res.insert(t);
		t = filenamefile(f.substr(0, n));
	}
	else {
		t = f;
	}

	gxFont* newFont = new gxFont(ftLibrary, this, f, height, bold, italic, underlined);
	font_set.emplace(newFont);
	return newFont;
}

gxFont* gxGraphics::verifyFont(gxFont* f) {
	return font_set.count(f) ? f : 0;
}

void gxGraphics::freeFont(gxFont* f) {
	if (font_set.erase(f)) delete f;
}