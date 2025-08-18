
#include "bbinput.h"

void InputInit()
{
    Input_Initialize(gx_runtime->hwnd);
}

void InputUpdate() {
    Input_Update();
}

int KeyHit(int key) {
    return Input_IsKeyPressed(key);
}

int KeyDown(int key) {
    return Input_IsKeyDown(key);
}

int KeyUp(int key) {
    return Input_IsKeyReleased(key);
}

bool input_create()
{
    return true;
}

bool input_destroy()
{
    return true;
}

void input_link(void (*rtSym)(const char* sym, void* pc)) {
    rtSym("InputInit", Input_Initialize);
    rtSym("InputUpdate", InputUpdate);
    rtSym("%KeyHit%key", KeyHit);
    rtSym("%KeyDown%key", KeyDown);
    rtSym("%KeyUp%key", KeyUp);

}