// Camera.h
#pragma once

struct float3 {
    float x, y, z;

    float3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

    // Operator overloads
    float3 operator+(const float3& rhs) const {
        return float3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    float3 operator-(const float3& rhs) const {
        return float3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    // Operator overload for scalar multiplication
    float3 operator*(float scalar) const {
        return float3(x * scalar, y * scalar, z * scalar);
    }
};

class Camera {
public:
    Camera();

    ~Camera();

    void setPosition(const float3& newPosition);
    void setDirection(const float3& newDirection);

    const float3& getPosition() const;
    const float3& getDirection() const;

private:
    float3 position;
    float3 direction;
};
