#include "Material.h"

bool Material::Initialize(D3D* d3d, LPCWSTR vertexPath, LPCWSTR pixelPath)
{
    bool success = false;

    mD3D = d3d;
    mD3Device = mD3D->GetDevice();
    mD3DevCon = mD3D->GetDevCon();

    success = InitializeShader(vertexPath, pixelPath);

    HRESULT hr = mD3Device->CreateInputLayout(
        mLayout.data(),
        mLayout.size(),
        mD3VSBuffer->GetBufferPointer(),
        mD3VSBuffer->GetBufferSize(),
        &mD3InputLayout);

    success = CheckDxError(hr, "Failed to create Input Layout!");

    success = D3D::CreateConstantBuffer<MatConstBuf>(mD3Device, &mD3ConstantBuffer);

    return success;
}

bool Material::Initialize(D3D* d3d, LPCWSTR vertexPath, LPCWSTR pixelPath, LPCWSTR texturePath)
{
    bool success = false;

    mD3D = d3d;
    mD3Device = mD3D->GetDevice();
    mD3DevCon = mD3D->GetDevCon();

    success = InitializeShader(vertexPath, pixelPath);

    HRESULT hr = mD3Device->CreateInputLayout(
        mLayout.data(),
        mLayout.size(),
        mD3VSBuffer->GetBufferPointer(),
        mD3VSBuffer->GetBufferSize(),
        &mD3InputLayout);

    success = CheckDxError(hr, "Failed to create Input Layout!");

    success = SetTexture(texturePath);

    return success;
}

void Material::Finalize()
{
    mD3InputLayout->Release();
    mD3InputLayout = nullptr;

    mD3PixelShader->Release();
    mD3PixelShader = nullptr;

    mD3VertexShader->Release();
    mD3VertexShader = nullptr;

    mD3PSBuffer->Release();
    mD3PSBuffer = nullptr;

    mD3VSBuffer->Release();
    mD3VSBuffer = nullptr;


}

void Material::Update(float deltaTime)
{

}

void Material::Draw(float deltaTime)
{
    mD3DevCon->VSSetShader(mD3VertexShader, nullptr, 0);
    mD3DevCon->PSSetShader(mD3PixelShader, nullptr, 0);

    mD3DevCon->IASetInputLayout(mD3InputLayout);

    mD3DevCon->UpdateSubresource(mD3ConstantBuffer, 0, nullptr, &mMatConstBuf, 0, 0);
    mD3DevCon->PSSetConstantBuffers(1, 1, &mD3ConstantBuffer);

    for (auto& it : mTextures)
    {
        if (it.second.mD3Texture && it.second.mD3Sampler)
        {
            mD3DevCon->PSSetShaderResources(it.first, 1, &it.second.mD3Texture);
            mD3DevCon->PSSetSamplers(it.first, 1, &it.second.mD3Sampler);
        }
    }

}

bool Material::SetTexture(LPCWSTR path, int slot)
{
    bool success = false;

    Texture t = {};

    success = D3D::CreateTextureAndSampler(path, mD3Device, &t.mD3Texture, &t.mD3Sampler);
    if (success)
    {
        mMatConstBuf.isTextureBound = true;
    }

    mTextures[slot] = t;

    return success;
}

bool Material::InitializeShader(LPCWSTR vertexPath, LPCWSTR pixelPath)
{
    bool success = false;
    HRESULT hr;

    D3DX11CompileFromFile(
        vertexPath,
        nullptr,
        nullptr,
        "VS",
        "vs_4_0",
        0, 0,
        nullptr,
        &mD3VSBuffer,
        nullptr,
        &hr);
    success = CheckDxError(hr, "Failed to compile vertex shader!");

    D3DX11CompileFromFile(
        pixelPath,
        nullptr,
        nullptr,
        "PS",
        "ps_4_0",
        0, 0,
        nullptr,
        &mD3PSBuffer,
        nullptr,
        &hr);
    success = CheckDxError(hr, "Failed to compile pixel shader!");

    hr = mD3Device->CreateVertexShader(
        mD3VSBuffer->GetBufferPointer(),
        mD3VSBuffer->GetBufferSize(),
        nullptr,
        &mD3VertexShader);
    success = CheckDxError(hr, "Failed to create vertex shader!");

    hr = mD3Device->CreatePixelShader(
        mD3PSBuffer->GetBufferPointer(),
        mD3PSBuffer->GetBufferSize(),
        nullptr,
        &mD3PixelShader);
    success = CheckDxError(hr, "Failed to create pixel shader!");

    if (success)
    {
        std::cout << "Shader creation successful!" << std::endl;
    }

    return success;
}
