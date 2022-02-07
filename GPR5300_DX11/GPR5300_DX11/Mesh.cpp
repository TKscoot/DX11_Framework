#include "Mesh.h"

bool Mesh::Initialize(D3D* d3d, Camera* camera, Material* material)
{
    bool success = false;

    CreateSphere(4.0f, 64);

    mCamera = camera;
    mMaterial = material;

    mD3D = d3d;
    mD3Device = mD3D->GetDevice();
    mD3DevCon = mD3D->GetDevCon();

    success = CreateVertexAndIndexBuffers();

    success = D3D::CreateConstantBuffer<PerMeshConstBuf>(mD3Device, &mD3ConstantBuffer);

    return success;
}

void Mesh::Finalize()
{
    mD3ConstantBuffer->Release();
    mD3ConstantBuffer = nullptr;

    mD3IndexBuffer->Release();
    mD3IndexBuffer = nullptr;

    mD3VertexBuffer->Release();
    mD3VertexBuffer = nullptr;
}

void Mesh::Update(float deltaTime)
{
    mMaterial->Update(deltaTime);

    mPerMeshConstBuf.world = XMMatrixTranspose(mTransform.GetWorldMatrix());

    XMMATRIX wvp = 
        mTransform.GetWorldMatrix() * 
        mCamera->GetViewMatrix() * 
        mCamera->GetProjectionMatrix();

    mPerMeshConstBuf.worldViewProjection = XMMatrixTranspose(wvp);

    XMFLOAT3 camPos = XMFLOAT3();
    XMStoreFloat3(&camPos, mCamera->GetPosition());
    mPerMeshConstBuf.cameraPos = camPos;
}

void Mesh::Draw(float deltaTime)
{
    mMaterial->Draw(deltaTime);

    const UINT stride = sizeof(Vertex);
    const UINT offset = 0;
    mD3DevCon->IASetVertexBuffers(0, 1, &mD3VertexBuffer, &stride, &offset);
    mD3DevCon->IASetIndexBuffer(mD3IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    mD3DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    mD3DevCon->UpdateSubresource(
        mD3ConstantBuffer,
        0, nullptr,
        &mPerMeshConstBuf,
        0, 0);

    mD3DevCon->VSSetConstantBuffers(0, 1, &mD3ConstantBuffer);

    mD3DevCon->DrawIndexed(mIndices.size(), 0, 0);
}

bool Mesh::CreateVertexAndIndexBuffers()
{
    HRESULT hr;
    bool success = false;

    // --- Vertex Buffer
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * mVertices.size();
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA vertexBufferData = {};
    vertexBufferData.pSysMem = mVertices.data();

    hr = mD3Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mD3VertexBuffer);
    success = CheckDxError(hr, "Failed to create vertex buffer!");

    // --- Index Buffer
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.ByteWidth = sizeof(UINT32) * mIndices.size();
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA indexBufferData = {};
    indexBufferData.pSysMem = mIndices.data();

    hr = mD3Device->CreateBuffer(&indexBufferDesc, &indexBufferData, &mD3IndexBuffer);
    success = CheckDxError(hr, "Failed to create index buffer!");


    if (success)
    {
        std::cout << "Created vertex and index buffers!" << std::endl;
    }


    return success;
}

void Mesh::CreateSphere(float diameter, int tesselation)
{
    mVertices.clear();
    mIndices.clear();

    if (tesselation < 3)
    {
        std::cout << "Tesselation must be at least 3!" << std::endl;
        return;
    }

    int verticalSegments = tesselation;
    int horizontalSegments = tesselation * 2;

    float radius = diameter / 2;

    for (size_t i = 0; i <= verticalSegments; i++)
    {
        float v = 1 - float(i) / float(verticalSegments);

        float latitude = (float(i) * XM_PI / float(verticalSegments)) - XM_PIDIV2;

        float dy, dxz;

        XMScalarSinCos(&dy, &dxz, latitude);

        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            float u = float(j) / float(horizontalSegments);

            float longitude = float(j) * XM_2PI / float(horizontalSegments);

            float dx, dz;

            XMScalarSinCos(&dx, &dz, longitude);

            dx *= dxz;
            dz *= dxz;


            XMFLOAT3 position;
            XMFLOAT3 normal;
            XMFLOAT2 uv;

            XMStoreFloat3(&normal, XMVectorSet(dx, dy, dz, 0.0f));

            XMStoreFloat3(&position, XMVectorScale(XMVectorSet(dx, dy, dz, 0.0f), radius));

            XMStoreFloat2(&uv, XMVectorSet(u, v, 0.0f, 0.0f));

            mVertices.push_back(Vertex(position, normal, uv));
        }
    }


    int stride = horizontalSegments + 1;

    for (size_t i = 0; i < verticalSegments; i++)
    {
        for (size_t j = 0; j < horizontalSegments; j++)
        {
            size_t nextI = i + 1;
            size_t nextJ = j + 1 % stride;


            mIndices.push_back(i * stride + j);
            mIndices.push_back(nextI * stride + j);
            mIndices.push_back(i * stride + nextJ);

            mIndices.push_back(i * stride + nextJ);
            mIndices.push_back(nextI * stride + j);
            mIndices.push_back(nextI * stride + nextJ);
        }
    }
}

