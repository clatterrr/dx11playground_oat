#ifndef PERLIN_H
#define PERLIN_H

#include "SysDef.h"

class Perlin
{
public:
	Perlin();
	~Perlin();

	float GetHeight(float x,float y);
	XMFLOAT2 FakeRandomValue(float x,float y);
};


#endif // !PERLIN_H




