#include "Camera.h"

bool Camera::Initialize(Window* window)
{
    mPosition = XMVectorSet(0.0f, 0.0f, -8.0f, 0.0f);
    mTarget   = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    mUp       = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    mView = XMMatrixLookAtLH(mPosition, mTarget, mUp);

    mProjection = XMMatrixPerspectiveFovLH(
        XMConvertToRadians(mFieldOfView),
        (float)window->GetWindowWidth() / (float)window->GetWindowHeight(),
        mNearPlane, mFarPlane);

    return true;
}

void Camera::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
    mPosition = XMVectorSet(x, y, z, 0.0f);
    mDirty = true;
}

XMVECTOR Camera::GetPosition()
{
    return mPosition;
}

void Camera::SetTarget(FLOAT x, FLOAT y, FLOAT z)
{
    mTarget = XMVectorSet(x, y, z, 0.0f);
    mDirty = true;
}

XMMATRIX Camera::GetViewMatrix()
{
    if (mDirty)
    {
        mView = XMMatrixLookAtLH(mPosition, mTarget, mUp);
        
        mDirty = false;
    }

    return mView;
}

XMMATRIX Camera::GetProjectionMatrix()
{
    return mProjection;
}
