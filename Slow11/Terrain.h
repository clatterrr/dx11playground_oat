#ifndef TERRAIN_H
#define TERRAIN_H


#include <d3d11.h>
#include "GeometryGenerator.h"


class Terrain
{
public:
	Terrain();
	~Terrain();

	void DrawTerrain(ID3D11Device* t_device, ID3D11DeviceContext* t_context, XMFLOAT3 camPos);
	void ComputeTerrain(ID3D11Device* t_device, ID3D11DeviceContext* t_context,XMFLOAT3 camPos);

	float lod0isize = 0;
	float lod1isize = 0;
};



#endif // TERRAIN_H



