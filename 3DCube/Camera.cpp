// Camera.cpp
#include "Camera.h"

Camera::Camera() : position(0.0f, 0.0f, -1.5f), direction(0.0f, -1.0f, 1.0f) {
    // Initialize with default position and direction
}

Camera::~Camera() {
    // Destructor code (if needed)
}

void Camera::setPosition(const float3& newPosition) {
    position = newPosition;
}

void Camera::setDirection(const float3& newDirection) {
    direction = newDirection;
}

const float3& Camera::getPosition() const {
    return position;
}

const float3& Camera::getDirection() const {
    return direction;
}
