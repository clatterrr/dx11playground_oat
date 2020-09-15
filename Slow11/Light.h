#ifndef LIGHT_H

#define LIGHT_H
#include "SysDef.h"
class Light
{
public:
	Light();
	~Light();
	struct PointLight
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		XMFLOAT3 direction;
		float pad;
	};

	Light::PointLight CreatePointLight();

};


#endif // !LIGHT_H


