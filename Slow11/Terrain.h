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

	float TileCenterX;
	float TileCenterY;

};



#endif // TERRAIN_H



