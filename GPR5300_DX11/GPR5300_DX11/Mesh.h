#pragma once
#include <array>

#include "D3D.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"

class Mesh
{
public:
	bool Initialize(D3D* d3d, Camera* camera, Material* material);

	void Finalize();

	void Update(float deltaTime);

	void Draw(float deltaTime);

	Transform& GetTransform() { return mTransform; }

private:
	bool CreateVertexAndIndexBuffers();

	void CreateSphere(float diameter, int tesselation);

	Transform mTransform = {};

	Camera* mCamera = nullptr;
	Material* mMaterial = nullptr;

	D3D* mD3D = nullptr;
	ID3D11Device* mD3Device = nullptr;
	ID3D11DeviceContext* mD3DevCon = nullptr;

	// Mesh data
	ID3D11Buffer* mD3VertexBuffer = nullptr;
	ID3D11Buffer* mD3IndexBuffer  = nullptr;

	// Constant Buffer
	ID3D11Buffer* mD3ConstantBuffer = nullptr;
	PerMeshConstBuf mPerMeshConstBuf = {};

	std::vector<Vertex> mVertices =
	{
		// Front Face
		//		x		y	   z	 u	   v	 nx ny   nz
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,  0, 0, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,  0, 0, -1.0f),
		Vertex(1.0f,   1.0f, -1.0f, 1.0f, 0.0f,  0, 0, -1.0f),
		Vertex(1.0f,  -1.0f, -1.0f, 1.0f, 1.0f,  0, 0, -1.0f),

		// Back Face
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,	0, 0, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f,	0, 0, 1.0f),
		Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f,	0, 0, 1.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,	0, 0, 1.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0, 1.0f, 0),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0, 1.0f, 0),
		Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f,  0, 1.0f, 0),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f,  0, 1.0f, 0),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0, -1.0f, 0),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f,  0, -1.0f, 0),
		Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0, -1.0f, 0),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0, -1.0f, 0),

		// Left Face
		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, -1.0f, 0, 0),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f, 0, 0),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0, 0),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 0, 0),

		// Right Face
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0, 0),
		Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0, 0),
		Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0, 0),
		Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0, 0),
	};

	std::vector<UINT32> mIndices = 
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	
};

