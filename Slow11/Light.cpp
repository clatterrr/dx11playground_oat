#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

Light::PointLight Light::CreatePointLight()
{
	PointLight pl;
	pl.ambient = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
	pl.position = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	pl.direction = XMFLOAT3(0.25F, 0.5F, -1.0f);
	pl.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	return pl;
}
