#ifndef _CAMERA_H
#define _CAMERA_H

#include <directxmath.h>

using namespace DirectX;
class Camera
{
public:
	Camera();
	~Camera();
	void Update();
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	XMVECTOR GetPosition();
	XMVECTOR GetRotation();
	XMMATRIX GetProjMatrix();
	XMMATRIX GetViewMatrix();
private:
	bool c_ViewDirty = true;

	XMFLOAT3 c_position;
	XMFLOAT3 c_rotation;
	XMMATRIX c_viewMatrix;
	XMMATRIX c_projMatrix;
	XMFLOAT4X4 c_orthMatrix;
};
#endif
