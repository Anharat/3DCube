// Window.cpp
#include "Window.h"

Window::Window(HINSTANCE hInstance, int nCmdShow, InputManager* inputManager)
    : m_inputManager(inputManager)  // Initialize the InputManager pointer
{
    if (!InitializeWindow(hInstance, nCmdShow)) {
        // Handle window initialization failure
    }
}

bool Window::InitializeWindow(HINSTANCE hInstance, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = Window::WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Calculate the required size of the window rectangle, based on the desired client rectangle size.
    RECT rect = { 0, 0, 700, 700 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, FALSE);

    m_hWnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"3DCube",               // Window text
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // Window style (non-resizable)
        CW_USEDEFAULT, CW_USEDEFAULT,   // Position (x, y)
        rect.right - rect.left,         // Width
        rect.bottom - rect.top,         // Height
        NULL,                           // Parent window    
        NULL,                           // Menu
        hInstance,                      // Instance handle
        this                            // Additional application data
    );

    if (m_hWnd == NULL) {
        return false;
    }

    // Set the InputManager instance as window user data
    SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(m_inputManager));

    ShowWindow(m_hWnd, nCmdShow);

    return true;
}

HWND Window::GetHandle() const {
    return m_hWnd;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // Retrieve the InputManager instance
    InputManager* inputManager = reinterpret_cast<InputManager*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message) {
    case WM_KEYDOWN:
        if (inputManager) {
            inputManager->HandleKeyPress(wParam);
        }
        break;

    case WM_CLOSE:
        PostQuitMessage(0);
        break;

        

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;  // Ensure that a value is returned for all code paths
}
