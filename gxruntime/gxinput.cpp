#include "gxinput.h"

typedef struct {
    BYTE currentKeys[256];
    BYTE previousKeys[256];

    POINT mousePosition;
    POINT previousMousePosition;
    int mouseButtons;
    int previousMouseButtons;
    int mouseWheelDelta;
} InputState;

static InputState s_input;
static HWND s_hwnd = NULL;

    void Input_Initialize(HWND hwnd) {
        s_hwnd = hwnd;
        memset(&s_input, 0, sizeof(InputState));

        GetKeyboardState(s_input.currentKeys);
        memcpy(s_input.previousKeys, s_input.currentKeys, 256);
    }

    void Input_Update() {
        if (!s_hwnd) return;

        memcpy(s_input.previousKeys, s_input.currentKeys, 256);
        s_input.previousMouseButtons = s_input.mouseButtons;
        s_input.previousMousePosition = s_input.mousePosition;

        GetKeyboardState(s_input.currentKeys);

        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(s_hwnd, &pt);
        s_input.mousePosition = pt;

        s_input.mouseWheelDelta = 0;
    }

    void Input_ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            if (msg == WM_LBUTTONDOWN)
                s_input.mouseButtons |= MOUSE_LEFT;
            else
                s_input.mouseButtons &= ~MOUSE_LEFT;
            break;

        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            if (msg == WM_RBUTTONDOWN)
                s_input.mouseButtons |= MOUSE_RIGHT;
            else
                s_input.mouseButtons &= ~MOUSE_RIGHT;
            break;

        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            if (msg == WM_MBUTTONDOWN)
                s_input.mouseButtons |= MOUSE_MIDDLE;
            else
                s_input.mouseButtons &= ~MOUSE_MIDDLE;
            break;

        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
            if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) {
                if (msg == WM_XBUTTONDOWN)
                    s_input.mouseButtons |= MOUSE_X1;
                else
                    s_input.mouseButtons &= ~MOUSE_X1;
            }
            else {
                if (msg == WM_XBUTTONDOWN)
                    s_input.mouseButtons |= MOUSE_X2;
                else
                    s_input.mouseButtons &= ~MOUSE_X2;
            }
            break;

        case WM_MOUSEWHEEL:
            s_input.mouseWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
            break;
        }
    }

    int Input_IsKeyDown(int vk_code) {
        return (s_input.currentKeys[vk_code] & 0x80) != 0;
    }

    int Input_IsKeyPressed(int vk_code) {
        return ((s_input.currentKeys[vk_code] & 0x80) != 0) &&
            ((s_input.previousKeys[vk_code] & 0x80) == 0);
    }

    int Input_IsKeyReleased(int vk_code) {
        return ((s_input.currentKeys[vk_code] & 0x80) == 0) &&
            ((s_input.previousKeys[vk_code] & 0x80) != 0);
    }

    POINT Input_GetMousePosition() {
        return s_input.mousePosition;
    }

    POINT Input_GetMouseDelta() {
        POINT delta = {
            s_input.mousePosition.x - s_input.previousMousePosition.x,
            s_input.mousePosition.y - s_input.previousMousePosition.y
        };
        return delta;
    }

    int Input_IsMouseButtonDown(int button) {
        return (s_input.mouseButtons & button) != 0;
    }

    int Input_IsMouseButtonPressed(int button) {
        return ((s_input.mouseButtons & button) != 0) &&
            ((s_input.previousMouseButtons & button) == 0);
    }

    int Input_IsMouseButtonReleased(int button) {
        return ((s_input.mouseButtons & button) == 0) &&
            ((s_input.previousMouseButtons & button) != 0);
    }

    int Input_GetMouseWheelDelta() {
        return s_input.mouseWheelDelta;
    }


