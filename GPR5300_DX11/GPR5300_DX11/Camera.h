#pragma once
#include <d3d11.h>
#include <D3Dx11.h>
#include <xnamath.h>

#include "Window.h"

class Camera
{
public:
	bool Initialize(Window* window);

	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	XMVECTOR GetPosition();

	void SetTarget(FLOAT x, FLOAT y, FLOAT z);

	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();

private:

	bool mDirty = false;

	XMVECTOR mPosition;
	XMVECTOR mTarget;
	XMVECTOR mUp;

	XMMATRIX mView;
	XMMATRIX mProjection;

	FLOAT mNearPlane	= 0.001f;
	FLOAT mFarPlane		= 1000.0f;
	FLOAT mFieldOfView	= 60.0f;
};

