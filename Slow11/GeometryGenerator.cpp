#include "GeometryGenerator.h"


GeometryGenerator::GeometryGenerator()
{
   
}

GeometryGenerator::~GeometryGenerator()
{
}

GeometryGenerator::MeshData GeometryGenerator::CreateBox(float px, float py, float pz, float width, float height, float depth)
{
    MeshData meshData;
    Vertex v[24];
    float w2 = 0.5f * width;
    float h2 = 0.5f * height;
    float d2 = -0.5f * depth;

    v[0] = Vertex(px - w2, py - h2, pz + d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f  ,0.0f,0.0f);
    v[1] = Vertex(px - w2, py + h2, pz + d2, 0.0f, 0.0f, 1.0f, 1.0f,0.0f,0.0f   ,0.0f,1.0f);
    v[2] = Vertex(px + w2, py + h2, pz + d2, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f ,1.0f,1.0f);
    v[3] = Vertex(px + w2, py - h2, pz + d2, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f ,1.0f,0.0f);

    v[4] = Vertex(px + w2, py - h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[5] = Vertex(px + w2, py + h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[6] = Vertex(px - w2, py + h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[7] = Vertex(px - w2, py - h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    v[8] = Vertex(px - w2, py + h2, pz + d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[9] = Vertex(px - w2, py + h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[10] = Vertex(px + w2, py + h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[11] = Vertex(px + w2, py + h2, pz + d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    v[12] = Vertex(px - w2, py - h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[13] = Vertex(px - w2, py - h2, pz + d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[14] = Vertex(px + w2, py - h2, pz + d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[15] = Vertex(px + w2, py - h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    v[16] = Vertex(px - w2, py - h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[17] = Vertex(px - w2, py + h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[18] = Vertex(px - w2, py + h2, pz + d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[19] = Vertex(px - w2, py - h2, pz + d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    v[20] = Vertex(px + w2, py - h2, pz + d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[21] = Vertex(px + w2, py + h2, pz + d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[22] = Vertex(px + w2, py + h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[23] = Vertex(px + w2, py - h2, pz - d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    meshData.Vertices.assign(&v[0], &v[24]);
    
    int i[36];

    // Fill in the front face index data
    i[0] = 0; i[1] = 1; i[2] = 2;
    i[3] = 0; i[4] = 2; i[5] = 3;

    // Fill in the back face index data
    i[6] = 4; i[7] = 5; i[8] = 6;
    i[9] = 4; i[10] = 6; i[11] = 7;

    // Fill in the top face index data
    i[12] = 8; i[13] = 9; i[14] = 10;
    i[15] = 8; i[16] = 10; i[17] = 11;

    // Fill in the bottom face index data
    i[18] = 12; i[19] = 13; i[20] = 14;
    i[21] = 12; i[22] = 14; i[23] = 15;

    // Fill in the left face index data
    i[24] = 16; i[25] = 17; i[26] = 18;
    i[27] = 16; i[28] = 18; i[29] = 19;

    // Fill in the right face index data
    i[30] = 20; i[31] = 21; i[32] = 22;
    i[33] = 20; i[34] = 22; i[35] = 23;

    meshData.Indices.assign(&i[0], &i[36]);

    return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateSphere(float px, float py, float pz, float r, int sliceCount, int stackCount)
{

    MeshData meshData;
    Vertex BotVertex = Vertex(px, py - r, pz, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    Vertex TopVertex = Vertex(px, py + r, pz, 0.0f, -1.0f, 0.0f, 1.0, 0.0f, 0.0f, 0.0f, 0.0f);
    if (stackCount < 5)
        stackCount = 5;
    if (sliceCount < 5)
        sliceCount = 5;

    float phiStep = XM_PI / stackCount;//南北极每一块的角度
    float thetaStep = 2.0f * XM_PI / sliceCount;//赤道每一块的角度
    meshData.Vertices.push_back(TopVertex);
    for (int i = 1; i < stackCount; i++)
    {
        float phi = i * phiStep;
        for (int j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;
            Vertex v;
            v.Position.x = px + r * sinf(phi) * cosf(theta);
            v.Position.y = py + r * cosf(phi);
            v.Position.z = pz + r * sinf(phi) * sinf(theta);

            v.Normal = XMFLOAT3(sinf(phi) * cosf(theta), cosf(phi), sinf(phi) * sinf(theta));
            v.UV.x = theta / XM_2PI;
            v.UV.y = phi / XM_PI;
            meshData.Vertices.push_back(v);
        }
    }
    meshData.Vertices.push_back(BotVertex);

    for (int i = 1; i <= sliceCount; i++)
    {
        meshData.Indices.push_back(0);
        meshData.Indices.push_back(i  + 1);
        meshData.Indices.push_back(i);
    }
    UINT baseIndex = 1;
    UINT ringVertexCount = sliceCount + 1;
    for (UINT i = 0; i < stackCount - 2; i++)
    {
        for (UINT j = 0; j < sliceCount; j++)
        {
            meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);
            meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

            meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
            meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }
    UINT southPoleIndex = (UINT)meshData.Vertices.size() - 1;
    baseIndex = southPoleIndex - ringVertexCount;

    for (UINT i = 0; i < sliceCount; i++)
    {
        meshData.Indices.push_back(southPoleIndex);
        meshData.Indices.push_back(baseIndex + i);
        meshData.Indices.push_back(baseIndex + i + 1);
    }


    return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateSqure(float px, float py, float pz, float length)
{
    MeshData meshData;

    std::vector<Vertex> v;

    v.resize(4);
    float halfLen = length / 2;
    v[0] = Vertex(px - halfLen, py , pz - halfLen, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[1] = Vertex(px - halfLen, py , pz + halfLen, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[2] = Vertex(px + halfLen, py, pz + halfLen, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
    v[3] = Vertex(px + halfLen, py , pz - halfLen, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);

    meshData.Vertices = v;

    int i[6];

    i[0] = 0; i[1] = 1; i[2] = 2;
    i[3] = 0; i[4] = 2; i[5] = 3;

    meshData.Indices.assign(&i[0], &i[6]);


    return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateATerrain(float TilePosX,float TilePosZ,float TileSize,float minSize,int edgecut)
{
    MeshData meshData;
    UINT Steps = TileSize / minSize;
    TilePosX -= TileSize / 2;
    TilePosZ -= TileSize / 2;
    //res = 0.5 hight res
    std::vector<Vertex> v;
    v.resize(Steps * Steps  * 6);
    std::vector<XMFLOAT3> FaceNormal;
    FaceNormal.resize(Steps * Steps * 2);
    for ( int i = 0; i < Steps; i++)
    {
        for (int j = 0; j < Steps; j++)
        {

            float x0 = TilePosX + i * minSize;
            float x1 = TilePosX + i * minSize + minSize;
            float y0 = TilePosZ + j * minSize + 2.0f;
            float y1 = TilePosZ + j * minSize + minSize + 2.0f;
            float h0 = Perlin_GetHeight(x0, y0);
            float h1 = Perlin_GetHeight(x0, y1);
            float h2 = Perlin_GetHeight(x1, y0);
            float h3 = Perlin_GetHeight(x1, y1);
            /*
            y1 1----------3
                 |              |
                 |              |
            y0 0----------2
                x0           x1
            */
            int baseidx = (i * Steps + j) * 6;
            if ((edgecut & 1) && j == Steps - 1)//上边的lod更加粗糙
            {
                if (i % 2 == 0)
                {
                    //处理 p3
                    h3 = (h1 + Perlin_GetHeight(x1 + minSize,y1)) / 2;
                }
                else
                {
                    h1 = (h3 + Perlin_GetHeight(x0 - minSize, y1)) / 2;
                }
            }
            if ((edgecut & 2) && j == 0)//下边的lod更加粗糙
            {
                if (i % 2 == 0)
                {
                    h2 = (h0 + Perlin_GetHeight(x1 + minSize, y0)) / 2;
                }
                else
                {
                    h0 = (h2 + Perlin_GetHeight(x0 - minSize, y0)) / 2;
                }
            }
            if ((edgecut & 4)&& (i == 0))//左边的lod更加粗糙
            {
                if (j % 2 == 0)
                {
                    h1 = (h0 + Perlin_GetHeight(x0, y1 + minSize)) / 2;
                }
                else
                {
                    h0 = (h1 + Perlin_GetHeight(x0 , y0 - minSize)) / 2;
                }
            }
            if ((edgecut & 8)&& (i == Steps - 1))//右边的lod更加粗糙
            {
                if (j % 2 == 0)
                {
                    h3 = (h2 + Perlin_GetHeight(x1, y1 + minSize)) / 2;
                }
                else
                {
                    h2 = (h3 + Perlin_GetHeight(x1, y0 - minSize)) / 2;
                }
            }
            //cross product
            float a0, a1, a2, b0, b1, b2, n0, n1, n2, n3, n4, n5;
            a0 = x0 - x0, a1 = h1 - h0, a2 = y1 - y0;//0 -- 1
            b0 = x1 - x0, b1 = h3 - h0, b2 = y1 - y0;//0 -- 3
            n0 = a1 * b2 - a2 * b1;
            n1 = a2 * b0 - a0 * b2;
            n2 = a0 * b1 - a1 * b0;
            FaceNormal.push_back(XMFLOAT3(n0, n1, n2));
            a0 = x1 - x0, a1 = h3 - h0, a2 = y1 - y0;//0 -- 3 这两个别弄混了，否则法线就反过去了
            b0 = x1 - x0, b1 = h2 - h0, b2 = y0 - y0;//0 -- 2
            n3 = a1 * b2 - a2 * b1;
            n4 = a2 * b0 - a0 * b2;
            n5 = a0 * b1 - a1 * b0;
            FaceNormal.push_back(XMFLOAT3(n3, n4, n5));
            v[baseidx + 0]  = Vertex(x0,h0,y0, n0, n1, n2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            v[baseidx + 1] = Vertex(x0,h1, y1, n0, n1, n2,0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
            v[baseidx + 2] = Vertex(x1, h3, y1, n0, n1, n2, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);

            v[baseidx + 3] = Vertex(x0, h0, y0, n3, n4, n5, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            v[baseidx + 4] = Vertex(x1, h3, y1, n3, n4, n5, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
            v[baseidx + 5] = Vertex(x1, h2, y0, n3, n4, n5, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);


        }
    }
    meshData.Vertices = v;
    std::vector<UINT> ind;
    ind.resize(Steps * Steps * 6);
    for (int ii = 0; ii < Steps; ii++)
    {
        for (int j = 0; j < Steps; j++)
        {
           // 0 1 2
            // 0 2 3
            
            UINT baseidx = (ii * Steps + j) * 6;
            UINT veridx = (ii * Steps + j) * 6;
            ind[baseidx] = veridx;
            ind[baseidx + 1] =  veridx + 1;
            ind[baseidx + 2] = veridx + 2;

            ind[baseidx + 3] = veridx  + 3;
            ind[baseidx + 4] = veridx + 4;
            ind[baseidx + 5] = veridx + 5;
          
        }
    }
    meshData.Indices = ind;
    return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateTerrainLOD()
{
    return MeshData();
}

float GeometryGenerator::Perlin_GetHeight(float x, float y)
{
  // return (sinf(x * 4.0f) + 1) * 0.1f;
  //  return x * 0.2f;
    XMFLOAT2 bottomleft = Perlin_FakeVector(x - 0.5, y - 0.5);
    XMFLOAT2 bottomright = Perlin_FakeVector(x + 0.5, y - 0.5);
    XMFLOAT2 topleft = Perlin_FakeVector(x - 0.5, y + 0.5);
    XMFLOAT2 topright = Perlin_FakeVector(x + 0.5, y + 0.5);
    float result = bottomleft.x + bottomleft.y + bottomright.x + bottomright.y + topleft.x + topleft.y + topright.x + topright.y;
    return result * 0.2f;
}
    

XMFLOAT2 GeometryGenerator::Perlin_FakeVector(float x, float y)
{

    float fakeX = sinf(x * 1785 + y * 8051) + cosf(3021 * x + 3245 * y);
    float fakeY = sinf(x * 5685 + y * 8211) + cosf(3121 * x + 4145 * y);
    float base = sqrt(fakeX * fakeX + fakeY * fakeY);
    return XMFLOAT2(fakeX, fakeY);
}

GeometryGenerator::Vertex::Vertex()
{
}
