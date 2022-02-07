#include "Application.h"

bool Application::Initialize(D3D* d3d, Window* window)
{
    bool success = false;

    mD3D = d3d;
    mD3Device = mD3D->GetDevice();
    mD3DevCon = mD3D->GetDevCon();

    success = D3D::CreateConstantBuffer<PerFrameConstBuf>(mD3Device, &mD3ConstantBuffer);

    mMaterial.Initialize(mD3D, TEXT("DefaultVertexShader.hlsl"), TEXT("LitPixelShader.hlsl"));
    mMaterial.SetTexture(TEXT("SAE_Logo.png"), 0);
    mMaterial.SetTexture(TEXT("red.png"), 1);
    mMaterial.SetTexture(TEXT("perlin.png"), 2);

    success = camera.Initialize(window);

    success = mCube.Initialize(d3d, &camera, &mMaterial);
    mCube.GetTransform().SetTranslation(-3.0f, 0.0f, 0.0f);
    //mCube.GetTransform().SetRotation({ 1.0f, 1.0f, 0.0, 0.0 }, 45.0f);

    success = mCube1.Initialize(d3d, &camera, &mMaterial);
    mCube1.GetTransform().SetTranslation(3.0f, 0.0f, 0.0f);
    mCube1.GetTransform().SetRotation({ 0.0f, 1.0f, 1.0, 0.0 }, 45.0f);

    return success;
}

void Application::Finalize()
{
    mMaterial.Finalize();
}

void Application::Update(float deltaTime)
{
    static float angle = 0.0f;

    mPerFrameConstBuf.time += deltaTime;

    angle += deltaTime * 0.05f;

    if (angle > 360.0f) angle = 0.0f;

    mCube.GetTransform().SetRotation({ 1.0f, 1.0f, 0.0f }, angle);

    mCube.Update(deltaTime);
    mCube1.Update(deltaTime);
}

void Application::Render(float deltaTime)
{
    mD3D->BeginRender();

    mD3DevCon->UpdateSubresource(mD3ConstantBuffer, 0, nullptr, &mPerFrameConstBuf, 0, 0);
    mD3DevCon->PSSetConstantBuffers(0, 1, &mD3ConstantBuffer);

    mCube.Draw(deltaTime);
    mCube1.Draw(deltaTime);

    mD3D->EndRender();
}

