// Window.h
#pragma once
#include <Windows.h>
#include "InputManager.h"


class Window {
public:
    Window(HINSTANCE hInstance, int nCmdShow, InputManager* inputManager);
    HWND GetHandle() const;
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    bool InitializeWindow(HINSTANCE hInstance, int nCmdShow);
    HWND m_hWnd = nullptr;
    InputManager* m_inputManager;
};
