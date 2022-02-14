#include "D3D.h"

bool D3D::Initialize(HINSTANCE hInstance, Window& window)
{
    bool success = false;

    // Backbuffer description
    DXGI_MODE_DESC backbufferDesc = {};
    backbufferDesc.Width  = window.GetWindowWidth();
    backbufferDesc.Height = window.GetWindowHeight();
    backbufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    backbufferDesc.RefreshRate.Numerator   = 60;
    backbufferDesc.RefreshRate.Denominator = 1;
    backbufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    backbufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    // Swapchain description
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc = backbufferDesc;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.Flags = NULL;
    swapChainDesc.OutputWindow = window.GetWindowHandle();
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Windowed = TRUE;

    // Creating Device, Device Context and Swap Chain
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        nullptr,
        NULL,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &mDxSwapChain,
        &mD3Device,
        NULL,
        &mD3DevCon);

    success = CheckDxError(hr, "Device and SwapChain creation Failed!");

    // Create Render Target View
    ID3D11Texture2D* backBufferTexture = nullptr;
    hr = mDxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);
    success = CheckDxError(hr, "Failed to get BackBuffer!");

    hr = mD3Device->CreateRenderTargetView(backBufferTexture, NULL, &mD3RenderTargetView);
    success = CheckDxError(hr, "Failed to Create RenderTargetView!");

    backBufferTexture->Release();
    backBufferTexture = nullptr;

    // TODO: Depth Stencil View erstellen 
    D3D11_TEXTURE2D_DESC depthStencilTexureDesc = {};
    depthStencilTexureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilTexureDesc.Width = window.GetWindowWidth();
    depthStencilTexureDesc.Height = window.GetWindowHeight();
    depthStencilTexureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilTexureDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilTexureDesc.ArraySize = 1;
    depthStencilTexureDesc.CPUAccessFlags = 0;
    depthStencilTexureDesc.MipLevels = 1;
    depthStencilTexureDesc.SampleDesc.Count = 1;
    depthStencilTexureDesc.SampleDesc.Quality = 0;
    depthStencilTexureDesc.MiscFlags = 0;

    mD3Device->CreateTexture2D(&depthStencilTexureDesc, nullptr, &mD3DepthStencilTexture);
    mD3Device->CreateDepthStencilView(mD3DepthStencilTexture, nullptr, &mD3DepthStencilView);

    mD3DevCon->OMSetRenderTargets(1, &mD3RenderTargetView, mD3DepthStencilView);

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    mD3Device->CreateDepthStencilState(&depthStencilDesc, &mD3DepthStencilState);

    mD3DevCon->OMSetDepthStencilState(mD3DepthStencilState, 1);

    CreateRasterizerStates();

    mD3DevCon->RSSetState(mD3SolidRasterizerState);

    // Setting Viewport
    D3D11_VIEWPORT viewport = {};
    viewport.Width = window.GetWindowWidth();
    viewport.Height = window.GetWindowHeight();
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    mD3DevCon->RSSetViewports(1, &viewport);

    if (success)
    {
        std::cout << "Direct3D initialization successful!" << std::endl;
    }

    return success;
}

void D3D::Finalize()
{
    mD3RenderTargetView->Release();
    mD3RenderTargetView = nullptr;

    mD3DevCon->Release();
    mD3DevCon = nullptr;

    mD3Device->Release();
    mD3Device = nullptr;

    mDxSwapChain->Release();
    mDxSwapChain = nullptr;

}

bool D3D::CreateTextureAndSampler(LPCWSTR path, ID3D11Device* device, ID3D11ShaderResourceView** outTexture, ID3D11SamplerState** outSampler)
{
    HRESULT hr;
    bool success = false;

    D3DX11CreateShaderResourceViewFromFile(device, path, nullptr, nullptr, outTexture, &hr);
    success = CheckDxError(hr, "Failed to create Shader Resource View!");

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = device->CreateSamplerState(&samplerDesc, outSampler);
    success = CheckDxError(hr, "Failed to create Sampler!");

    return success;
}

bool D3D::CreateRasterizerStates()
{
    HRESULT hr;
    bool success = false;

    D3D11_RASTERIZER_DESC rasterizerDesc = {};

    // Solid & Both faces
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = false;
    
    hr = mD3Device->CreateRasterizerState(&rasterizerDesc, &mD3SolidRasterizerState);
    success = CheckDxError(hr, "Failed to create Solid Rasterizer State!");

    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = false;

    hr = mD3Device->CreateRasterizerState(&rasterizerDesc, &mD3WireframeRasterizerState);
    success = CheckDxError(hr, "Failed to create Solid Rasterizer State!");


    return success;
}
