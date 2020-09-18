#ifndef SYSDEF_H
#define SYSDEF_H

#include <windows.h>
#include <wrl/client.h>
#include <string>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <directxcolors.h>
#include <memory>

#include "Camera.h"
#include "GeometryGenerator.h"
#include "Input.h"
#include "Light.h"
//extern library
#include "WICTextureLoader.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

using namespace DirectX;
using namespace Microsoft::WRL;



#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800

struct Vertex
{
    Vertex() {}
    Vertex(float x, float y, float z)
        :pos(x, y, z) {}
    XMFLOAT3 pos;
    XMFLOAT3 nor;
    XMFLOAT3 col;
    XMFLOAT2 uv;
};

#endif SYSDEF_H
