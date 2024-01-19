// Renderer.cpp

#include "Renderer.h"

Renderer::Renderer(HWND hWnd) {
    InitDirect3D(hWnd);
    InitShaders();
    InitFullScreenQuad();
    InitConstantBuffer(hWnd);
    
}

Renderer::~Renderer() {
    // Release the Direct3D resources
    if (vertexBuffer) vertexBuffer->Release();
    if (inputLayout) inputLayout->Release();
    if (vertexShader) vertexShader->Release();
    if (pixelShader) pixelShader->Release();
    if (renderTargetView) renderTargetView->Release();
    if (swapChain) swapChain->Release();
    if (deviceContext) deviceContext->Release();
    if (device) device->Release();
    if (cameraBuffer) cameraBuffer->Release();
}

void Renderer::InitDirect3D(HWND hWnd) {
    // Get the client area dimensions
    RECT rect;
    GetClientRect(hWnd, &rect);
    UINT width = rect.right - rect.left;
    UINT height = rect.bottom - rect.top;

    // Device and swap chain creation
    DXGI_SWAP_CHAIN_DESC scd;
    // Initialize the swap chain description
    ZeroMemory(&scd, sizeof(scd));
    scd.BufferCount = 1;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;  // or set according to your requirements
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;

    // Create the device and swap chain
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &scd, &swapChain, &device, nullptr, &deviceContext);

    if (FAILED(hr)) {
        // Handle errors
        return;
    }

    // Get the back buffer and create the render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));

    if (FAILED(hr) || pBackBuffer == nullptr) {
        // Handle the error.
        return;
    }

    hr = device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);
    pBackBuffer->Release(); // Release pBackBuffer after using it

    if (FAILED(hr)) {
        // Handle the error.
        return;
    }

    // Set the render target
    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

    // Set up the viewport
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    deviceContext->RSSetViewports(1, &viewport);
}

void Renderer::InitShaders() {
    // Compile the vertex shader
    ID3DBlob* vertexShaderBlob;
    D3DCompileFromFile(L"SimpleVertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &vertexShaderBlob, nullptr);



    // Create the vertex shader
    device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);

    // Compile the pixel shader
    ID3DBlob* pixelShaderBlob;
    D3DCompileFromFile(L"SimplePixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &pixelShaderBlob, nullptr);


    // Create the pixel shader
    device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);

    // Define the vertex input layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
    device->CreateInputLayout(layout, numElements, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);

    // Release the shader blobs
    vertexShaderBlob->Release();
    pixelShaderBlob->Release();
}

void Renderer::InitFullScreenQuad() {
    // Define the vertices of the full-screen quad (two triangles)
    Vertex quadVertices[] = {
        { DirectX::XMFLOAT3(-1.0f,  1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) }, // Top Left
        { DirectX::XMFLOAT3(1.0f,  1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) }, // Top Right
        { DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }, // Bottom Left
        { DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }  // Bottom Right
    };

    // Create a vertex buffer description
    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(quadVertices);
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    // Set up the subresource data
    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = quadVertices;

    // Create the vertex buffer
    HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
    if (FAILED(hr)) {
        // Handle errors (e.g., MessageBox)
        return;
    }
}

void Renderer::InitConstantBuffer(HWND hWnd) {
    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd, sizeof(cbd));
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.ByteWidth = sizeof(CameraBuffer); // Ensure this is a multiple of 16
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT hr = device->CreateBuffer(&cbd, nullptr, &cameraBuffer);
    if (FAILED(hr)) {
        // Enhanced error handling
        wchar_t errorMsg[256];
        swprintf_s(errorMsg, L"Failed to create camera buffer. HRESULT: 0x%X", hr);
        MessageBox(hWnd, errorMsg, L"Error", MB_OK);
        return;
    }
}




void Renderer::UpdateBuffer(const DirectX::XMFLOAT3& cameraPosition, const DirectX::XMFLOAT3& cameraDirection) {
    if (!cameraBuffer) {
        MessageBox(NULL, L"Camera buffer is not initialized.", L"Error", MB_OK);
        return;
    }

    CameraBuffer cb;
    cb.cameraPosition = cameraPosition;
    cb.cameraDirection = cameraDirection;

    deviceContext->UpdateSubresource(cameraBuffer, 0, nullptr, &cb, 0, 0);
}




ID3D11Buffer* Renderer::GetCameraBuffer() const {
    return cameraBuffer;
}


ID3D11Buffer* Renderer::GetVertexBuffer() const {
    return vertexBuffer;
}

ID3D11InputLayout* Renderer::GetInputLayout() const {
    return inputLayout;
}

ID3D11VertexShader* Renderer::GetVertexShader() const {
    return vertexShader;
}

ID3D11PixelShader* Renderer::GetPixelShader() const {
    return pixelShader;
}

ID3D11RenderTargetView* Renderer::GetRenderTargetView() const {
    return renderTargetView;
}

ID3D11DeviceContext* Renderer::GetDeviceContext() const {
    return deviceContext;
}

IDXGISwapChain* Renderer::GetSwapChain() const {
    return swapChain;
}