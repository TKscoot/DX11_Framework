#pragma once
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <xnamath.h>

#include <iostream>
#include <string>
#include <vector>

struct Vertex 
{
	Vertex() {}
	Vertex(
		float x, float y, float z,		// Position
		float u, float v,				// Texturkoordinaten (UVs)
		float nx, float ny, float nz)	// Normals
		: pos(x, y, z)
		, texCoords(u, v)
		, normal(nx, ny, nz)
	{
	}

	Vertex(XMFLOAT3 pos, XMFLOAT3 normal, XMFLOAT2 uv) 
		:pos(pos)
		,normal(normal)
		,texCoords(uv)
	{

	}

	XMFLOAT3 pos; // position
	XMFLOAT2 texCoords; // Texturkoordinaten (UVs)
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 bitangent;
	
};

struct PerMeshConstBuf 
{
	XMMATRIX worldViewProjection = XMMatrixIdentity();
	XMMATRIX world = XMMatrixIdentity();
	XMFLOAT3 cameraPos;
	FLOAT padding;
};

struct Light
{
	XMFLOAT4 ambient   = { 0.2f,  0.2f,  0.2f, 1.0f };
	XMFLOAT4 diffuse   = { 1.0f,  1.0f,  1.0f, 1.0f };
	XMFLOAT4 specular  = { 1.0f,  1.0f,  1.0f, 1.0f };
	XMFLOAT3 direction = { 0.2f, -0.5f, -1.0f};
	float intensity	   = 0.8f;
};

struct Texture 
{
	ID3D11ShaderResourceView* mD3Texture = nullptr;
	ID3D11SamplerState* mD3Sampler = nullptr;
};

struct PerFrameConstBuf 
{
	Light light;
	float time;
	float pad0;
	float pad1;
	float pad2;
};

inline bool CheckDxError(HRESULT hr, std::string message = "Default Message!") 
{
	if (hr != S_OK)
	{
		std::cout << "ERROR! HRESULT: " << hr << "---" << message << std::endl;
		return false;
	}
	else
	{
		return true;
	}

	return false;
}