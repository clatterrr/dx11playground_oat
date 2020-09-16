#include "GeometryGenerator.h"

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

GeometryGenerator::MeshData GeometryGenerator::CreateSphere(float px, float py, float pz, float r, int latitude, int magnitude)
{
    MeshData meshData;
    Vertex BotVertex = Vertex(px, py - r, pz, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    Vertex TopVertex = Vertex(px, py + r, pz, 0.0f, -1.0f, 0.0f, 1.0, 0.0f, 0.0f, 0.0f, 0.0f);
    if (latitude < 5)
        latitude = 5;
    if (magnitude < 5)
        magnitude = 5;

    float phiStep = XM_PI / magnitude;//南北极每一块的角度
    float thetaStep = 2.0f * XM_PI / latitude;//赤道每一块的角度
    float alength = 2 * r / magnitude;
    meshData.Vertices.push_back(TopVertex);
    for (int i = 1; i < magnitude; i++)
    {
        float phi = i * phiStep;
        for (int j = 0; j <= latitude; j++)
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

    for (int i = 1; i < latitude; i++)
    {
        meshData.Indices.push_back(0);
        meshData.Indices.push_back(i + 1);
        meshData.Indices.push_back(i);
    }
    UINT baseIndex = 1;
    UINT ringVertexCount = latitude + 1;
    for (UINT i = 0; i < magnitude - 2; i++)
    {
        for (UINT j = 0; j < latitude; j++)
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

    for (UINT i = 0; i < latitude; i++)
    {
        meshData.Indices.push_back(southPoleIndex);
        meshData.Indices.push_back(baseIndex + i);
        meshData.Indices.push_back(baseIndex + i + 1);
    }


    return meshData;
}

GeometryGenerator::Vertex::Vertex()
{
}
