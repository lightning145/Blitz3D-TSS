#include "std.h"
#include "bbgraphics.h"
#include "bbinput.h"
#include "../gxruntime/gxutf8.h"
#include "../MultiLang/MultiLang.h"

#include <glad/glad.h>

#include "../blitz3d/ShaderFile.h"
#include "../blitz3d/Font.h"
#include "../blitz3d/FrameBuffer.h"
#include "../blitz3d/Animator.h"
#include "../blitz3d/stb_image.h"
#include "../blitz3d/GameTimer.h"

using namespace MD_Math;

FT_Library ft;


void Graphics(int w, int h)
{
    gx_runtime->InitWindow(w, h);
}

int GetWindowWidth()
{
    return gx_runtime->GetWidth();
}

int GetWindowHeight()
{
    return gx_runtime->GetHeight();
}

void SetBuffer()
{
    FrameBuffer::InitBuffer(GetWindowWidth(), GetWindowHeight());
}

void FreeBuffer()
{
    FrameBuffer::FreeBuffer();
}

void Present()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    FrameBuffer::FBshader.Use();
    glBindVertexArray(FrameBuffer::quadVAO);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, FrameBuffer::texture_fbo);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, FrameBuffer::texture_fbo);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    gx_runtime->SwapBackBuffer();
}

void SetViewPort(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void Clear(int red, int green, int blue)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer::FBO);
    glClearColor(float(red) / 255.0f, (float)green / 255.0f, (float)blue / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

Font* LoadFont(BBStr* fontpath, int size)
{
    Font* font = new Font(fontpath->c_str(), size, ft);
    delete fontpath;
    return font;
}

void SetFont(Font* font)
{
    font->Set();
}

void TextDraw(Font* font, int x, int y, BBStr* str)
{
    font->Text(str->c_str());

    text1Shader.Link();
    text1Shader.Use();
    text1Shader.SetMatrix("projection", PerspectiveMatrixRH(AngularToRadian(45.0f), (float)GetWindowWidth() / (float)GetWindowHeight(), 0.1f, 100.0f));
    text1Shader.SetMatrix("view", ViewMatrixRH(VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f)));
    text1Shader.SetMatrix("model", IdentityMatrix());
    text1Shader.SetVec3("textColor", VECTOR3(1.0f, 1.0f, 1.0f));

    font->Draw((float)x / (float)GetWindowWidth(), (float)y / (float)GetWindowHeight(), 0.0f, 0.003f);
    delete str;
}

void FreeFont(Font* font)
{
    delete font;
}

GameTimer* CreateGameTimer()
{
    GameTimer* timer = new GameTimer();
    return timer;
}

void TimeReset(GameTimer* timer)
{
    timer->Reset();
}

void TimePause(GameTimer* timer)
{
    timer->Pause();
}

void TimeResume(GameTimer* timer)
{
    timer->Resume();
}

float getCurrentTime(GameTimer* timer)
{
    return (float)timer->getCurrentTime();
}

float getDeltaTime(GameTimer* timer)
{
    return timer->GetDeltaTime();
}

void TimeIsPaused(GameTimer* timer)
{
    timer->IsPaused();
}

void FreeGameTimer(GameTimer* time)
{
    delete time;
}

bool graphics_create()
{
    gx_runtime->InitWindow(400, 300);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    FT_Init_FreeType(&ft);
    stbi_set_flip_vertically_on_load(true);
    
    return true;
}

bool graphics_destroy()
{
    
    gx_runtime->FreeWindow();
    FT_Done_FreeType(ft);
    return true;
}

void graphics_link(void (*rtSym)(const char* sym, void* pc))
{

    rtSym("Graphics%w%h", Graphics);
    rtSym("Present", Present);
    rtSym("Clear%r%g%b", Clear);
    rtSym("SetViewport%x%y%w%h", SetViewPort);

    rtSym("%LoadFont$fontpath%size", LoadFont);
    rtSym("SetFont%fontpath", SetFont);
    rtSym("TextDraw%font%x%y$text", TextDraw);
    rtSym("FreeFont%fontpath", FreeFont);

    rtSym("%GetWindowWidth", GetWindowWidth);
    rtSym("%GetWindowHeight", GetWindowHeight);

    rtSym("SetBuffer", SetBuffer);
    rtSym("FreeBuffer", FreeBuffer);

    rtSym("%CreateGameTimer", CreateGameTimer);
    rtSym("TimeReset%timer", TimeReset);
    rtSym("TimePause%timer", TimePause);
    rtSym("TimeResume%timer", TimeResume);
    rtSym("#getCurrentTime%timer", getCurrentTime);
    rtSym("#getDeltaTime%timer", getDeltaTime);
    rtSym("TimeIsPaused%timer", TimeIsPaused);
    rtSym("FreeGameTimer%timer", FreeGameTimer);

}

extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    _declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}