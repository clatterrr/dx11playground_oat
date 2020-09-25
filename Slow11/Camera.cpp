#include "Camera.h"

Camera::Camera()
{
	c_position = XMFLOAT3(0, 0, 0);
	c_rotation = XMFLOAT3(0, 0, 0);
	c_forward = XMFLOAT3(0, 0, 1);
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

XMFLOAT3 Camera::GetPosition()
{
	return c_position;
}

XMFLOAT3 Camera::GetRotation()
{
	return c_rotation;
}

XMMATRIX Camera::GetProjMatrix()
{
	XMVECTOR  up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR lookAt = XMLoadFloat3(&c_forward);
	XMVECTOR pos = XMLoadFloat3(&c_position);
	float pitch = c_rotation.x;
	float yaw = c_rotation.y;
	float roll = c_rotation.z;	// 0
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);
	lookAt = pos + lookAt;
	c_viewMatrix = XMMatrixLookAtLH(pos, lookAt, up);
	c_projMatrix = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)800 / 800, 0.1f, 1000.0f);
	return c_projMatrix;
}

XMMATRIX Camera::GetViewMatrix()
{
	return c_viewMatrix;
}
