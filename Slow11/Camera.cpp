#include "Camera.h"

Camera::Camera()
{
	c_position = XMFLOAT3(0, 0, 0);
	c_rotation = XMFLOAT3(0, 0, 0);
}

Camera::~Camera()
{
}

void Camera::Update()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	c_position = XMFLOAT3(x, y, z);
	c_ViewDirty = true;
}

void Camera::SetRotation(float x, float y, float z)
{
	c_rotation = XMFLOAT3(x, y, z);
	c_ViewDirty = true;
}

XMVECTOR Camera::GetPosition()
{
	return XMLoadFloat3(&c_position);
}

XMVECTOR Camera::GetRotation()
{
	return XMLoadFloat3(&c_rotation);
}

XMMATRIX Camera::GetProjMatrix()
{
	XMVECTOR  up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR pos = XMLoadFloat3(&c_position);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(c_rotation.x, c_rotation.y, c_rotation.z);
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);
	lookAt = pos + lookAt;
	c_viewMatrix = XMMatrixLookAtLH(pos, lookAt, up);
	c_projMatrix = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)800 / 600, 1.0f, 1000.0f);
	return c_projMatrix;
}

XMMATRIX Camera::GetViewMatrix()
{
	return c_viewMatrix;
}
