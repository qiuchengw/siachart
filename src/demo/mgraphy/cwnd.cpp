#include "cwnd.h"

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <memory>

static int gWindowX = CW_USEDEFAULT;
static int gWindowY = 0;
static int gWindowWidth = CW_USEDEFAULT;
static int gWindowHeight = 0;

void CWindow::closeWindow() {
    RECT r;
    if (GetWindowRect(fHWnd, &r)) {
        gWindowX = r.left;
        gWindowY = r.top;
        gWindowWidth = r.right - r.left;
        gWindowHeight = r.bottom - r.top;
    }
    DestroyWindow(fHWnd);
}

CWindow::~CWindow() {
    this->closeWindow();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool CWindow::init(HINSTANCE hInstance) {
    fHInstance = hInstance ? hInstance : GetModuleHandle(nullptr);

    // The main window class name
    static const TCHAR gSZWindowClass[] = _T("SkiaApp");

    static WNDCLASSEX wcex;
    static bool wcexInit = false;
    if (!wcexInit) {
        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = fHInstance;
        wcex.hIcon = LoadIcon(fHInstance, (LPCTSTR)IDI_WINLOGO);
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);;
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = gSZWindowClass;
        wcex.hIconSm = LoadIcon(fHInstance, (LPCTSTR)IDI_WINLOGO);;

        if (!RegisterClassEx(&wcex)) {
            return false;
        }
        wcexInit = true;
    }

    /*
     if (fullscreen)
     {
         DEVMODE dmScreenSettings;
         // If full screen set the screen to maximum size of the users desktop and 32bit.
         memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
         dmScreenSettings.dmSize = sizeof(dmScreenSettings);
         dmScreenSettings.dmPelsWidth = (unsigned long)width;
         dmScreenSettings.dmPelsHeight = (unsigned long)height;
         dmScreenSettings.dmBitsPerPel = 32;
         dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

         // Change the display settings to full screen.
         ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

         // Set the position of the window to the top left corner.
         posX = posY = 0;
     }
     */
//   gIsFullscreen = fullscreen;

    fHWnd = CreateWindow(gSZWindowClass, nullptr, WS_OVERLAPPEDWINDOW,
                         gWindowX, gWindowY, gWindowWidth, gWindowHeight,
                         nullptr, nullptr, fHInstance, nullptr);
    if (!fHWnd) {
        return false;
    }

    SetWindowLongPtr(fHWnd, GWLP_USERDATA, (LONG_PTR)this);
    RegisterTouchWindow(fHWnd, 0);

    onCreated();

    return true;
}

static uint32_t get_modifiers(UINT message, WPARAM wParam, LPARAM lParam) {
    uint32_t modifiers = 0;

    switch (message) {
    case WM_UNICHAR:
    case WM_CHAR:
        if (0 == (lParam & (1 << 30))) {
            modifiers |= sia::kFirstPress_ModifierKey;
        }
        if (lParam & (1 << 29)) {
            modifiers |= sia::kOption_ModifierKey;
        }
        break;

    case WM_KEYDOWN: {
        SHORT sta = GetKeyState(VK_CONTROL);
        if (HIBYTE(sta)) {
            modifiers |= sia::kControl_ModifierKey;
        }
        sta = GetKeyState(VK_SHIFT);
        if (HIBYTE(sta)) {
            modifiers |= sia::kShift_ModifierKey;
        }
    }// fall through
    case WM_SYSKEYDOWN:
        if (0 == (lParam & (1 << 30))) {
            modifiers |= sia::kFirstPress_ModifierKey;
        }
        if (lParam & (1 << 29)) {
            modifiers |= sia::kOption_ModifierKey;
        }
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        if (lParam & (1 << 29)) {
            modifiers |= sia::kOption_ModifierKey;
        }
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL:
        if (wParam & MK_CONTROL) {
            modifiers |= sia::kControl_ModifierKey;
        }
        if (wParam & MK_SHIFT) {
            modifiers |= sia::kShift_ModifierKey;
        }
        break;
    }
    return modifiers;
}


static sia::KeyBoard get_key(WPARAM vk) {
    static const struct {
        WPARAM      fVK;
        sia::KeyBoard fKey;
    } gPair[] = {
        { VK_BACK, sia::KeyBoard::kBack },
        { VK_CLEAR, sia::KeyBoard::kBack },
        { VK_RETURN, sia::KeyBoard::kOK },
        { VK_UP, sia::KeyBoard::kUp },
        { VK_DOWN, sia::KeyBoard::kDown },
        { VK_LEFT, sia::KeyBoard::kLeft },
        { VK_RIGHT, sia::KeyBoard::kRight },
        { VK_TAB, sia::KeyBoard::kTab },
        { VK_PRIOR, sia::KeyBoard::kPageUp },
        { VK_NEXT, sia::KeyBoard::kPageDown },
        { VK_HOME, sia::KeyBoard::kHome },
        { VK_END, sia::KeyBoard::kEnd },
        { VK_DELETE, sia::KeyBoard::kDelete },
        { VK_ESCAPE, sia::KeyBoard::kEscape },
        { VK_SHIFT, sia::KeyBoard::kShift },
        { VK_CONTROL, sia::KeyBoard::kCtrl },
        { VK_MENU, sia::KeyBoard::kOption },
        { 'A', sia::KeyBoard::kA },
        { 'C', sia::KeyBoard::kC },
        { 'V', sia::KeyBoard::kV },
        { 'X', sia::KeyBoard::kX },
        { 'Y', sia::KeyBoard::kY },
        { 'Z', sia::KeyBoard::kZ },
    };
    for (size_t i = 0; i < SK_ARRAY_COUNT(gPair); i++) {
        if (gPair[i].fVK == vk) {
            return gPair[i].fKey;
        }
    }
    return sia::KeyBoard::kNONE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;

    CWindow* window = (CWindow*) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    bool eventHandled = false;

    switch (message) {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        window->onPaint();
        EndPaint(hWnd, &ps);
        eventHandled = true;
        break;

    case WM_CLOSE:
        PostQuitMessage(0);
        eventHandled = true;
        break;

    case WM_ACTIVATE:
        // disable/enable rendering here, depending on wParam != WA_INACTIVE
        break;

    case WM_SIZE:
        window->onResize(LOWORD(lParam), HIWORD(lParam));
        eventHandled = true;
        break;

    case WM_UNICHAR:
//             eventHandled = window->onChar((SkUnichar)wParam,
//                                           get_modifiers(message, wParam, lParam));
        break;

    case WM_CHAR: {
//         const uint16_t* c = reinterpret_cast<uint16_t*>(&wParam);
//         eventHandled = window->onChar(SkUTF16_NextUnichar(&c),
//                                         get_modifiers(message, wParam, lParam));
    } break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        eventHandled = window->onKey(get_key(wParam), sia::InputState::kDown_InputState,
                                     get_modifiers(message, wParam, lParam));
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
//             eventHandled = window->onKey(get_key(wParam), Window::kUp_InputState,
//                                          get_modifiers(message, wParam, lParam));
        break;

    case WM_LBUTTONDBLCLK: {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        eventHandled = window->onMouse(xPos, yPos, sia::InputState::kDoubleClick_InputState,
                                       get_modifiers(message, wParam, lParam));
    }
    break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP: {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        //if (!gIsFullscreen)
        //{
        //    RECT rc = { 0, 0, 640, 480 };
        //    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        //    xPos -= rc.left;
        //    yPos -= rc.top;
        //}

        sia::InputState istate = ((wParam & MK_LBUTTON) != 0) ? sia::InputState::kDown_InputState
                                 : sia::InputState::kUp_InputState;

        eventHandled = window->onMouse(xPos, yPos, istate,
                                       get_modifiers(message, wParam, lParam));
    }
    break;

    case WM_MOUSEMOVE: {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

//         if (!gIsFullscreen)
//         {
//            RECT rc = { 0, 0, 640, 480 };
//            AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
//            xPos -= rc.left;
//            yPos -= rc.top;
//         }

        eventHandled = window->onMouse(xPos, yPos, sia::InputState::kMove_InputState,
                                       get_modifiers(message, wParam, lParam));
    }
    break;

    case WM_MOUSEWHEEL:
//             eventHandled = window->onMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f,
//                                                 get_modifiers(message, wParam, lParam));
        break;

    case WM_TOUCH: {
//             uint16_t numInputs = LOWORD(wParam);
//             std::unique_ptr<TOUCHINPUT[]> inputs(new TOUCHINPUT[numInputs]);
//             if (GetTouchInputInfo((HTOUCHINPUT)lParam, numInputs, inputs.get(),
//                                   sizeof(TOUCHINPUT))) {
//                 POINT topLeft = {0, 0};
//                 ClientToScreen(hWnd, &topLeft);
//                 for (uint16_t i = 0; i < numInputs; ++i) {
//                     TOUCHINPUT ti = inputs[i];
//                     Window::InputState state;
//                     if (ti.dwFlags & TOUCHEVENTF_DOWN) {
//                         state = Window::kDown_InputState;
//                     } else if (ti.dwFlags & TOUCHEVENTF_MOVE) {
//                         state = Window::kMove_InputState;
//                     } else if (ti.dwFlags & TOUCHEVENTF_UP) {
//                         state = Window::kUp_InputState;
//                     } else {
//                         continue;
//                     }
//                     // TOUCHINPUT coordinates are in 100ths of pixels
//                     // Adjust for that, and make them window relative
//                     LONG tx = (ti.x / 100) - topLeft.x;
//                     LONG ty = (ti.y / 100) - topLeft.y;
//                     eventHandled = window->onTouch(ti.dwID, state, tx, ty) || eventHandled;
//                 }
//             }
    } break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return eventHandled ? 0 : 1;
}

void CWindow::setTitle(const char* title) {
    SetWindowTextA(fHWnd, title);
}

void CWindow::show() {
    ShowWindow(fHWnd, SW_SHOW);
}

void CWindow::onPaint() {
}

void CWindow::onResize(uint16_t w, uint16_t h) {
}

bool CWindow::onMouse(uint16_t x, uint16_t y, sia::InputState state, uint32_t mod) {
    return false;
}

bool CWindow::onKey(sia::KeyBoard k, sia::InputState state, uint32_t mod) {
    return true;
}

void CWindow::onIdle() {
    // onInval();
}

void CWindow::onInval() {
    InvalidateRect(fHWnd, nullptr, false);
}
