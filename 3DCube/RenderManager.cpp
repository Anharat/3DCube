// RenderManager.cpp

#include "RenderManager.h"

RenderManager::RenderManager(HWND hWnd) {
    renderer = new Renderer(hWnd);
}

RenderManager::~RenderManager() {
    if (renderer) {
        delete renderer;
    }
}

void RenderManager::Render(const DirectX::XMFLOAT3& cameraPosition, const DirectX::XMFLOAT3& cameraDirection) {
    auto deviceContext = renderer->GetDeviceContext();
    auto swapChain = renderer->GetSwapChain();
    auto renderTargetView = renderer->GetRenderTargetView();
    auto vertexBuffer = renderer->GetVertexBuffer();
    auto inputLayout = renderer->GetInputLayout();
    auto vertexShader = renderer->GetVertexShader();
    auto pixelShader = renderer->GetPixelShader();
    auto cameraBuffer = renderer->GetCameraBuffer();

    // Update the buffer with camera data
    renderer->UpdateBuffer(cameraPosition, cameraDirection);

    // Bind the constant buffer to the vertex shader
    UINT bufferNumber = 0;  // The slot where the buffer will be bound.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &cameraBuffer);

    // Clear the back buffer to a specific color
    float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // Grey color
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

    // Set the vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    // Set the input layout
    deviceContext->IASetInputLayout(inputLayout);

    // Set the primitive topology
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // Set the vertex and pixel shaders
    deviceContext->VSSetShader(vertexShader, nullptr, 0);
    deviceContext->PSSetShader(pixelShader, nullptr, 0);
    deviceContext->PSSetConstantBuffers(0, 1, &cameraBuffer);


    // Draw the quad
    deviceContext->Draw(4, 0);

    // Present the back buffer to the screen
    swapChain->Present(0, 0);
}

