// InputManager.h
#pragma once
#include <Windows.h>
#include "Camera.h"  // Include the Camera header

struct CameraData {
    float3 position;
    float3 direction;
};


class InputManager {
public:
    InputManager(Camera* camera);  // Update constructor
    ~InputManager();

    
    void HandleKeyPress(WPARAM wParam);
    CameraData Update();

private:
    Camera* m_camera;  // Camera pointer
};