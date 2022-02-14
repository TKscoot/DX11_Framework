#pragma once
#include "Material.h"
class SkyboxMaterial : public Material
{
public:
	bool Initialize(D3D* d3d, LPCWSTR cubemapPath, LPCWSTR vertexPath, LPCWSTR pixelPath) override;
	void Finalize() override;
	void Update(float deltaTime) override;
	void Draw(float deltaTime) override;

private:
	ID3D11ShaderResourceView* mD3CubeMapSRV		= nullptr;
	ID3D11SamplerState*		  mD3CubeMapSampler = nullptr;

	ID3D11DepthStencilState*  mD3DepthStencilState = nullptr;
};

