#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include <vector>
#include "SysDef.h"



class GeometryGenerator
{

public:

	GeometryGenerator();
	~GeometryGenerator();
	struct Vertex
	{
		Vertex();
		Vertex(float px, float py, float pz,
			float nx,float ny,float nz,
			float cr,float cg,float cb,
			float u,float v)
		{
			Position = XMFLOAT3(px, py, pz);
			Normal = XMFLOAT3(nx, ny, nz);
			Color = XMFLOAT3(cr, cg, cb);
			UV = XMFLOAT2(u, v);
		}
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT3 Color;
		XMFLOAT2 UV;
	};
	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices;
	};
	MeshData CreateBox(float px, float py, float pz, float width, float height, float depth);

	MeshData CreateSphere(float px, float py, float pz, float r, int latitude, int magnitude);

	MeshData CreateSqure(float px, float py, float pz, float length);

	MeshData CreateTerrain(float TilePosX, float TilePosZ, float TileSize, float minSize);

	float Perlin_GetHeight(int x, int y);
	XMFLOAT2 Perlin_FakeVector(float x, float y);
private:

};


#endif // !GEOMETRYGENERATOR_H



