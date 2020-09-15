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

GeometryGenerator::Vertex::Vertex()
{
}
