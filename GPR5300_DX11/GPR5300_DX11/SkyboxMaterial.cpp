#include "SkyboxMaterial.h"

bool SkyboxMaterial::Initialize(D3D* d3d, LPCWSTR cubemapPath, LPCWSTR vertexPath, LPCWSTR pixelPath)
{
    Material::Initialize(d3d, vertexPath, pixelPath);

    HRESULT hr;
    bool success = false;

    // Cubemap laden
    D3DX11_IMAGE_LOAD_INFO loadInfo = {};
    loadInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    ID3D11Texture2D* cubeMapTexture = nullptr;
    hr = D3DX11CreateTextureFromFile(mD3Device, cubemapPath, &loadInfo, nullptr, 
        (ID3D11Resource**)&cubeMapTexture, &hr);
    success = CheckDxError(hr, "Failed to create Cube Map!");

    D3D11_TEXTURE2D_DESC textureDesc;
    cubeMapTexture->GetDesc(&textureDesc);

    // Schauen ob Textur auch wirklich eine Cubemap ist wenn ArraySize == 6
    std::cout << "Texture Array Size: " << textureDesc.ArraySize << std::endl;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MipLevels = textureDesc.MipLevels;
    srvDesc.TextureCube.MostDetailedMip = 0;

    hr = mD3Device->CreateShaderResourceView(cubeMapTexture, &srvDesc, &mD3CubeMapSRV);
    success = CheckDxError(hr, "Failed to create Cube Map SRV!");

    // Sampler
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = mD3Device->CreateSamplerState(&samplerDesc, &mD3CubeMapSampler);
    success = CheckDxError(hr, "Failed to create Cube Map Sampler State!");

    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    hr = mD3Device->CreateDepthStencilState(&dsDesc, &mD3DepthStencilState);
    success = CheckDxError(hr, "Failed to create Skybox Depth Stencil State!");

    return success;
}

void SkyboxMaterial::Finalize()
{
    Material::Finalize();

    mD3DepthStencilState->Release();
    mD3DepthStencilState = nullptr;

    mD3CubeMapSampler->Release();
    mD3CubeMapSampler = nullptr;

    mD3CubeMapSRV->Release();
    mD3CubeMapSRV = nullptr;
}

void SkyboxMaterial::Update(float deltaTime)
{
}

void SkyboxMaterial::Draw(float deltaTime)
{
    mD3DevCon->IASetInputLayout(mD3InputLayout);
    mD3DevCon->VSSetShader(mD3VertexShader, nullptr, 0);
    mD3DevCon->PSSetShader(mD3PixelShader, nullptr, 0);

    //mD3DevCon->OMSetDepthStencilState(mD3DepthStencilState, 1);

    mD3DevCon->PSSetShaderResources(0, 1, &mD3CubeMapSRV);
    mD3DevCon->PSSetSamplers(0, 1, &mD3CubeMapSampler);
}
