#include "Application.h"

bool Application::Initialize(D3D* d3d, Window* window)
{
    bool success = false;

    mD3D = d3d;
    mD3Device = mD3D->GetDevice();
    mD3DevCon = mD3D->GetDevCon();

    success = D3D::CreateConstantBuffer<PerFrameConstBuf>(mD3Device, &mD3ConstantBuffer);

    mMaterial.Initialize(mD3D, TEXT("DefaultVertexShader.hlsl"), TEXT("LitPixelShader.hlsl"));
    mMaterial.SetTexture(TEXT("red.png"), 0);
    mMaterial.SetTexture(TEXT("normal_4.png"), 1);
    mMaterial.SetTexture(TEXT("perlin.png"), 2);

    mSkyboxMaterial.Initialize(d3d, TEXT("Skybox.dds"), 
        TEXT("SkyboxVertexShader.hlsl"), TEXT("SkyboxPixelShader.hlsl"));

    mSkySphere.Initialize(d3d, &camera, &mSkyboxMaterial);
    mSkySphere.GetTransform().SetScale(50.0f, 50.0f, 50.0f);

    success = camera.Initialize(window);
    camera.SetPosition(0.0f, 0.0f, -12.0f);

    // Skybox Position erst ändern, wenn Position von Kamera festgelegt wurde
    mSkySphere.GetTransform().SetTranslation(camera.GetPosition());

    success = mMesh1.Initialize(d3d, &camera, &mMaterial);
    mMesh1.GetTransform().SetTranslation(-3.0f, 0.0f, 0.0f);
    //mCube.GetTransform().SetRotation({ 1.0f, 1.0f, 0.0, 0.0 }, 45.0f);

    success = mMesh2.Initialize(d3d, "teapot.obj", & camera, &mMaterial);
    mMesh2.GetTransform().SetTranslation(3.0f, -1.5f, 0.0f);
    //mMesh2.GetTransform().SetScale(3.0f, 3.0f, 3.0f);
    //mMesh2.GetTransform().SetRotation({ 0.0f, 1.0f, 1.0, 0.0 }, 45.0f);

    return success;
}

void Application::Finalize()
{
    mMaterial.Finalize();

    mMesh1.Finalize();
    mMesh2.Finalize();

    mSkyboxMaterial.Finalize();
    mSkySphere.Finalize();
}

void Application::Update(float deltaTime)
{
    static float angle = 0.0f;

    mPerFrameConstBuf.time += deltaTime;

    angle += deltaTime * 0.05f;

    if (angle > 360.0f) angle = 0.0f;

    mMesh1.GetTransform().SetRotation({ 1.0f, 1.0f, 0.0f }, angle);

    mMesh1.Update(deltaTime);
    mMesh2.Update(deltaTime);
    mSkySphere.Update(deltaTime);
}

void Application::Render(float deltaTime)
{
    mD3D->BeginRender();

    mD3DevCon->UpdateSubresource(mD3ConstantBuffer, 0, nullptr, &mPerFrameConstBuf, 0, 0);
    mD3DevCon->PSSetConstantBuffers(0, 1, &mD3ConstantBuffer);

    mD3D->SetRasterizerState(D3D::RasterizerState::CULL_BACK);
    mMesh1.Draw(deltaTime);
    mMesh2.Draw(deltaTime);

    mD3D->SetRasterizerState(D3D::RasterizerState::CULL_FRONT);
    mSkySphere.Draw(deltaTime);

    mD3D->EndRender();
}

