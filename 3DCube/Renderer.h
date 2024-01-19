// Renderer.h

#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wchar.h>


struct Vertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 texcoord;
};

struct CameraBuffer {
    DirectX::XMFLOAT3 cameraPosition;
    float padding1;  // Padding to align to 16 bytes
    DirectX::XMFLOAT3 cameraDirection;
    float padding2;  // Additional padding to ensure 16-byte alignment
};


class Renderer {
public:
    Renderer(HWND hWnd);
    ~Renderer();

    ID3D11DeviceContext* GetDeviceContext() const;
    IDXGISwapChain* GetSwapChain() const;
    ID3D11Buffer* GetVertexBuffer() const;
    ID3D11InputLayout* GetInputLayout() const;
    ID3D11VertexShader* GetVertexShader() const;
    ID3D11PixelShader* GetPixelShader() const;
    ID3D11RenderTargetView* GetRenderTargetView() const;
    ID3D11Buffer* GetCameraBuffer() const;
    void UpdateBuffer(const DirectX::XMFLOAT3& cameraPosition, const DirectX::XMFLOAT3& cameraDirection);

private:
    void InitDirect3D(HWND hWnd);
    void InitShaders();
    void InitFullScreenQuad();
    void InitConstantBuffer(HWND hWnd);

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11Buffer* vertexBuffer;
    ID3D11InputLayout* inputLayout;
    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11Buffer* cameraBuffer;
};
