// App.cpp
#include "App.h"

App::App(HINSTANCE hInstance, int nCmdShow)
    : m_hInstance(hInstance),
    m_nCmdShow(nCmdShow),
    m_camera(),
    m_inputManager(&m_camera), 
    m_window(hInstance, nCmdShow, &m_inputManager),
    m_renderManager(m_window.GetHandle())
{
    // Rest of the constructor
}


int App::Run() {
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        CameraData cameraData = m_inputManager.Update();

        // Convert CameraData to DirectX::XMFLOAT3
        DirectX::XMFLOAT3 dxPosition(cameraData.position.x, cameraData.position.y, cameraData.position.z);
        DirectX::XMFLOAT3 dxDirection(cameraData.direction.x, cameraData.direction.y, cameraData.direction.z);

        m_renderManager.Render(dxPosition, dxDirection);
    }

    return static_cast<int>(msg.wParam);
}

