// InputManager.cpp
#include "InputManager.h"

InputManager::InputManager(Camera* camera) : m_camera(camera) {
    // Ensure camera is valid, handle accordingly
}

InputManager::~InputManager() {
    // Destructor code
}

CameraData InputManager::Update() {
    // Existing input handling logic...

    if (m_camera) {
        return { m_camera->getPosition(), m_camera->getDirection() };
    }
    else {
        return { float3(), float3() };  // Return default values if camera is null
    }
}


void InputManager::HandleKeyPress(WPARAM wParam) {
    const float deltaMovement = 0.05f;  // Movement speed
    const float deltaRotation = 0.05f;  // Rotation speed

    if (!m_camera) return;

    switch (wParam) {
        // Direction Changes
    case 'W':
        // Tilt camera direction upwards
        m_camera->setDirection(m_camera->getDirection() + float3(0.0f, deltaRotation, 0.0f));
        break;
    case 'S':
        // Tilt camera direction downwards
        m_camera->setDirection(m_camera->getDirection() - float3(0.0f, deltaRotation, 0.0f));
        break;
    case 'A':
        // Pan camera direction left
        m_camera->setDirection(m_camera->getDirection() - float3(deltaRotation, 0.0f, 0.0f));
        break;
    case 'D':
        // Pan camera direction right
        m_camera->setDirection(m_camera->getDirection() + float3(deltaRotation, 0.0f, 0.0f));
        break;

        // Position Changes
    case VK_UP:
        // Move camera position forward
        m_camera->setPosition(m_camera->getPosition() + m_camera->getDirection() * deltaMovement);
        break;
    case VK_DOWN:
        // Move camera position backward
        m_camera->setPosition(m_camera->getPosition() - m_camera->getDirection() * deltaMovement);
        break;
    case VK_LEFT:
        // Strafe camera position left
        // Assuming left is perpendicular to the current direction on the XZ plane
        m_camera->setPosition(m_camera->getPosition() - float3(deltaMovement, 0.0f, 0.0f));
        break;
    case VK_RIGHT:
        // Strafe camera position right
        // Assuming right is perpendicular to the current direction on the XZ plane
        m_camera->setPosition(m_camera->getPosition() + float3(deltaMovement, 0.0f, 0.0f));
        break;
    }
}


