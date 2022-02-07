#pragma once
#include <d3d11.h>
#include <D3Dx11.h>
#include <xnamath.h>

class Transform
{
public:
	Transform();

	void SetTranslation(FLOAT x, FLOAT y, FLOAT z);
	void SetRotation(FXMVECTOR axis, FLOAT angle);
	void SetScale(FLOAT x, FLOAT y, FLOAT z);

	XMMATRIX GetWorldMatrix();

private:
	XMMATRIX mTranslation;
	XMMATRIX mRotation;
	XMMATRIX mScale;
};

