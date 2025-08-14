#ifndef GXGRAPHICS_H
#define GXGRAPHICS_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <set>
#include <string>

#include "ddutil.h"

#include "gxfont.h"
#include "gxcanvas.h"
#include "gxmovie.h"

class gxRuntime;

class gxGraphics {
public:
	IDirectDraw7* dirDraw;
	IDirectDraw* ds_dirDraw;

	DDPIXELFORMAT primFmt, zbuffFmt;

	DDPIXELFORMAT texRGBFmt[2], texAlphaFmt[2], texRGBAlphaFmt[2], texRGBMaskFmt[2];

	FT_Library ftLibrary;

	bool running_on_wine;

	gxGraphics(gxRuntime* runtime, IDirectDraw7* dirDraw, IDirectDrawSurface7* front, IDirectDrawSurface7* back, bool d3d);
	~gxGraphics();

	bool restore();

	gxRuntime* runtime;
	//std::set<std::set<std::any>*> custom_set;

private:

	gxCanvas* front_canvas, * back_canvas;
	gxFont* def_font;
	bool gfx_lost;

	std::set<gxFont*> font_set;
	std::set<gxCanvas*> canvas_set;
	std::set<gxMovie*> movie_set;
	std::set<std::string> font_res;

	DDGAMMARAMP _gammaRamp;
	IDirectDrawGammaControl* _gamma;

	/***** GX INTERFACE *****/
public:
	enum {
		GRAPHICS_WINDOWED = 1,	//windowed mode
		GRAPHICS_SCALED = 2,		//scaled window
		GRAPHICS_3D = 4,			//3d mode! Hurrah!
		GRAPHICS_AUTOSUSPEND = 8,	//suspend graphics when app suspended
		GRAPHICS_BORDERLESS = 16
	};

	//MANIPULATORS
	void vwait();
	void flip(bool vwait);

	//SPECIAL!
	void copy(gxCanvas* dest, int dx, int dy, int dw, int dh, gxCanvas* src, int sx, int sy, int sw, int sh);

	//NEW! Gamma control!
	void setGamma(int r, int g, int b, float dr, float dg, float db);
	void getGamma(int r, int g, int b, float* dr, float* dg, float* db);
	void updateGamma(bool calibrate);

	//ACCESSORS
	int getWidth()const;
	int getHeight()const;
	int getDepth()const;
	int getScanLine()const;
	int getAvailVidmem()const;
	int getTotalVidmem()const;

	gxCanvas* getFrontCanvas()const;
	gxCanvas* getBackCanvas()const;
	gxFont* getDefaultFont()const;

	//OBJECTS
	gxCanvas* createCanvas(int width, int height, int flags);
	gxCanvas* loadCanvas(const std::string& file, int flags);
	gxCanvas* verifyCanvas(gxCanvas* canvas);
	void freeCanvas(gxCanvas* canvas);

	gxMovie* openMovie(const std::string& file, int flags);
	gxMovie* verifyMovie(gxMovie* movie);
	void closeMovie(gxMovie* movie);

	gxFont* loadFont(std::string font, int height, bool bold = false, bool italic = false, bool underlined = false);
	gxFont* verifyFont(gxFont* font);
	void freeFont(gxFont* font);

};

#endif