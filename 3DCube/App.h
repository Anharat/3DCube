// App.h
#pragma once
#include <Windows.h>
#include "Window.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "Camera.h"

               
class App {
public:
    App(HINSTANCE hInstance, int nCmdShow);
    int Run();

private:
    HINSTANCE m_hInstance = nullptr;
    int m_nCmdShow = 0;
    Window m_window;
    RenderManager m_renderManager;
    InputManager m_inputManager;
    Camera m_camera;

};
