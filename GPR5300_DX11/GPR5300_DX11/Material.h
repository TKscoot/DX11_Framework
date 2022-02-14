#pragma once
#include "D3D.h"
#include <unordered_map>

class Material
{
public:
	virtual bool Initialize(D3D* d3d, LPCWSTR vertexPath, LPCWSTR pixelPath);
	virtual bool Initialize(D3D* d3d, LPCWSTR vertexPath, LPCWSTR pixelPath, LPCWSTR texturePath);
	virtual void Finalize();
	virtual void Update(float deltaTime);
	virtual void Draw(float deltaTime);

	bool SetTexture(LPCWSTR path, int slot = 0);

protected:
	bool InitializeShader(LPCWSTR vertexPath, LPCWSTR pixelPath);

	D3D* mD3D = nullptr;
	ID3D11Device* mD3Device = nullptr;
	ID3D11DeviceContext* mD3DevCon = nullptr;

	// Shader kram
	ID3D10Blob* mD3VSBuffer = nullptr;
	ID3D10Blob* mD3PSBuffer = nullptr;
	ID3D11VertexShader* mD3VertexShader = nullptr;
	ID3D11PixelShader* mD3PixelShader = nullptr;

	// Material Constant Buffer
	struct MatConstBuf
	{
		XMFLOAT4 albedo = { 0.0, 1.0, 1.0, 1.0 };
		bool isTextureBound = false;
		float pad0;
		float pad1;
		float pad2;
	};
	MatConstBuf mMatConstBuf = {};
	ID3D11Buffer* mD3ConstantBuffer = nullptr;


	// Texture
	std::unordered_map<int, Texture> mTextures;

	//Input Layout
	ID3D11InputLayout* mD3InputLayout = nullptr;

	std::vector<D3D11_INPUT_ELEMENT_DESC> mLayout =
	{
		{
			"POSITION",						//LPCSTR SemanticName;
			0,								//UINT SemanticIndex;
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGI_FORMAT Format;
			0,								//UINT InputSlot;
			0,								//UINT AlignedByteOffset;
			D3D11_INPUT_PER_VERTEX_DATA,	//D3D11_INPUT_CLASSIFICATION InputSlotClass;
			0								//UINT InstanceDataStepRate;
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};
};

