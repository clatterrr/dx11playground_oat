#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include <vector>
#include "SysDef.h"

class GeometryGenerator
{
public:
	struct Vertex
	{
		Vertex();
		Vertex(float px, float py, float pz,
			float nx,float ny,float nz,
			float cr,float cg,float cb)
		{
			Position = XMFLOAT3(px, py, pz);
			Normal = XMFLOAT3(nx, ny, nz);
			Color = XMFLOAT3(cr, cg, cb);
		}
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT3 Color;
	};
	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices;
	};
	MeshData CreateBox(float px, float py, float pz, float width, float height, float depth);
};


#endif // !GEOMETRYGENERATOR_H



