#include "Terrain.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

void Terrain::DrawTerrain(ID3D11Device* t_device,ID3D11DeviceContext* t_context,XMFLOAT3 camPos)
{
    ID3D11Buffer* triangleVertBuffer;
    ID3D11Buffer* IndexBuffer;
    std::vector<GeometryGenerator::MeshData> meshs;
    meshs.empty();
    float camx = camPos.x;
    float camz = camPos.z;
    int CenterTileX = (int)(camx * 50) / 80 ;
    int CenterTileZ = (int)(camz * 50) / 80 + 1;
    float tileSize = 4.0f;

    GeometryGenerator geoGen;
 //   meshs.push_back(geoGen.CreateSqure(CenterTileX * 1.6f, -0.3f, CenterTileZ * 1.6f, tileSize));
    TileCenterX = (int)camx;
    TileCenterY =  (int)camz;
    meshs.push_back(geoGen.CreateTerrain(TileCenterX, TileCenterY,tileSize,1.0f));
    UINT vsize = 0, isize = 0, vi = 0, vk = 0;
   

    for (int i = 0; i < meshs.size(); i++)
    {
        vsize += meshs[i].Vertices.size();
        isize += meshs[i].Indices.size();
    }
    std::vector<Vertex> v;
    v.resize(vsize);
    std::vector<UINT> indices;
    indices.resize(isize);

    for (int i = 0; i < meshs.size(); i++)
    {

        for (int j = 0; j < meshs[i].Indices.size(); j++)
        {
            indices[vi] = meshs[i].Indices[j] + vk;
            vi++;
        }
        for (int j = 0; j < meshs[i].Vertices.size(); j++)
        {
            v[vk].pos = meshs[i].Vertices[j].Position;
            v[vk].nor = meshs[i].Vertices[j].Normal;
            v[vk].col = meshs[i].Vertices[j].Color;
            v[vk].uv = meshs[i].Vertices[j].UV;
            vk++;
        }
    }
    {
        //totalIndices = isize;
        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(UINT) * isize;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA idxData;
        idxData.pSysMem = &indices[0];
        t_device->CreateBuffer(&indexBufferDesc, &idxData, &IndexBuffer);
       t_context->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
       //this would clear the old index buffer,so old object cannot be drawn.
       //drawindexed would take the new index and clear the old one,but not drawn
       //present is actually drawn

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        vertexBufferDesc.ByteWidth = sizeof(Vertex) * vsize;//不能用 sizeof v,否则大小就变成了0
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferData;

        ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
        vertexBufferData.pSysMem = &v[0];
        t_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &triangleVertBuffer);
    }
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    t_context->IASetVertexBuffers(0, 1, &triangleVertBuffer, &stride, &offset);
    t_context->DrawIndexed(isize, 0, 0);

    triangleVertBuffer->Release();//otherwise create buffer every frame take your program into freeze
    IndexBuffer->Release();
}

void Terrain::ComputeTerrain(ID3D11Device* t_device, ID3D11DeviceContext* t_context,XMFLOAT3 camPos)
{


    //DrawTerrain(t_device, t_context);
    //create high res near terrain if distance to center is less than 5
    //tile size = 1.6,quad size = 0.1

    //create medium res terrain
    //tile size = 1.6 quad size = 0.2

    //create low res far terrain
    //tile size = 1.6 quad size = 0.4
}
