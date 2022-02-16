#pragma once
#include "D3D.h"
#include "Mesh.h"
#include "Camera.h"
#include "SkyboxMaterial.h"


class Application
{
public:
	bool Initialize(D3D* d3d, Window* window);

	void Finalize();

	void Update(float deltaTime);

	void Render(float deltaTime);

private:

	// Meshes
	Mesh mMesh1  = {};
	Mesh mMesh2 = {};

	Mesh mSkySphere = {};

	// Materials
	Material mMaterial = {};
	SkyboxMaterial mSkyboxMaterial = {};

	Camera camera = {};

	ID3D11Buffer* mD3ConstantBuffer = nullptr;
	PerFrameConstBuf mPerFrameConstBuf = {};

	D3D* mD3D = nullptr;
	ID3D11Device* mD3Device = nullptr;
	ID3D11DeviceContext* mD3DevCon = nullptr;
};

