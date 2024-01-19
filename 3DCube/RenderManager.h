// RenderManager.h

#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Renderer.h"
#include "InputManager.h"

class RenderManager {
public:
    RenderManager(HWND hWnd);
    ~RenderManager();
    void Render(const DirectX::XMFLOAT3& cameraPosition, const DirectX::XMFLOAT3& cameraDirection);


private:
    Renderer* renderer = nullptr;
};