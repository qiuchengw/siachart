#pragma once

#include <windows.h>
#include <stdint.h>
#include "graphy/event.h"

class CWindow {
    friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    ~CWindow();

    bool init(HINSTANCE instance);

    void setTitle(const char* title);
    void show();
    void onIdle();
    void onInval();

    HWND hWnd()const {
        return fHWnd;
    }

protected:
    virtual void onCreated() {
    }

    virtual void onPaint();

    virtual void onResize(uint16_t w, uint16_t h);

    virtual bool onMouse(uint16_t x, uint16_t y, sia::InputState state, uint32_t mod);

    virtual bool onKey(sia::KeyBoard k, sia::InputState state, uint32_t mod);

private:
    void closeWindow();

    HINSTANCE fHInstance = NULL;
    HWND      fHWnd = NULL;
};
