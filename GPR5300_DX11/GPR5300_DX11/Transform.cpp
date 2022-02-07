#include "Transform.h"

Transform::Transform()
{
    mTranslation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
    mRotation    = XMMatrixIdentity();
    mScale       = XMMatrixScaling(1.0f, 1.0f, 1.0f);
}

void Transform::SetTranslation(FLOAT x, FLOAT y, FLOAT z)
{
    mTranslation = XMMatrixTranslation(x, y, z);
}

void Transform::SetRotation(FXMVECTOR axis, FLOAT angle)
{
    mRotation = XMMatrixRotationAxis(axis, XMConvertToRadians(angle));
}

void Transform::SetScale(FLOAT x, FLOAT y, FLOAT z)
{
    mScale = XMMatrixScaling(x, y, z);
}

XMMATRIX Transform::GetWorldMatrix()
{
    return mScale * mRotation * mTranslation;
}
