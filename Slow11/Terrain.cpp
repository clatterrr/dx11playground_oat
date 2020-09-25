#include "Terrain.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

void Terrain::DrawTerrain(ID3D11Device* t_device, ID3D11DeviceContext* t_context, XMFLOAT3 camPos)
{
    ID3D11Buffer* triangleVertBuffer;
    ID3D11Buffer* IndexBuffer;
    std::vector<GeometryGenerator::MeshData> meshs;
    meshs.empty();
    float camx = camPos.x;
    float camz = camPos.z;
   // camx = camz = 0.0f;
    float tileSize = 4.0f;

    GeometryGenerator geoGen;
    float TileCenterX = (int)camx;
    float TileCenterY =  (int)camz;
    float lod0size = 1.0f / 1.0f;
    int edgecut = 15;
    meshs.push_back(geoGen.CreateATerrain(TileCenterX, TileCenterY,tileSize,lod0size, edgecut));
    edgecut = 0;
    float lod1size = lod0size * 2;
    meshs.push_back(geoGen.CreateATerrain(TileCenterX - tileSize, TileCenterY + tileSize, tileSize, lod1size,edgecut));//up left
    meshs.push_back(geoGen.CreateATerrain(TileCenterX,               TileCenterY + tileSize, tileSize, lod1size, edgecut));//up medium
    meshs.push_back(geoGen.CreateATerrain(TileCenterX + tileSize, TileCenterY + tileSize, tileSize, lod1size, edgecut));//up right
    meshs.push_back(geoGen.CreateATerrain(TileCenterX - tileSize, TileCenterY, tileSize, lod1size, edgecut));//left
    meshs.push_back(geoGen.CreateATerrain(TileCenterX + tileSize, TileCenterY, tileSize, lod1size, edgecut));//right
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
  //  t_context->DrawIndexed(meshs[0].Indices.size(), 0, 0);
  //  t_context->DrawIndexed(isize - meshs[0].Indices.size(), meshs[0].Indices.size(), 0);
    lod0size = meshs[0].Indices.size();
    lod1isize = isize;
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
