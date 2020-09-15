//commit 2

#include "SysDef.h"


IDXGISwapChain* sp_swapchain;
ID3D11Device* sp_device;
ID3D11DeviceContext* sp_context;
ID3D11RenderTargetView* sp_rtv;

ID3D11Buffer* triangleVertBuffer;
ID3D11Buffer* IndexBuffer;
ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthStencilBuffer;
ComPtr<ID3D11InputLayout> sp_VertexLayout;

// 安全COM组件释放宏
#define SAFE_RELEASE(p) { if ((p)) { (p)->Release(); (p) = nullptr; } }


float red = 0.0f, green = 0.0f, blue = 0.0f;
int colormodr = 1, colormodg = 1, colormodeb = 1;

const int Width = 800;
const int Height = 600;

LPCTSTR g_szClassName = L"BzTutsApp";
WNDCLASSEX wc;
HWND hwnd;
MSG Msg;
HRESULT hr;

//08-Camera
ID3D11Buffer* simpleCBuffer;
Camera camera;
struct TransformCBuffer
{
    XMMATRIX MVP;
} tcbuffer;

//09-Input
Input i_input;


bool InitWinWindow(HINSTANCE hInstance, int nCmdShow);
bool Initd3dWindow(HINSTANCE hInstance);
HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
HRESULT CreateShaderFromFile(
    const WCHAR* csoFileNameInOut,
    const WCHAR* hlslFileName,
    LPCSTR entryPoint,
    LPCSTR shaderModel,
    ID3DBlob** ppBlobOut);
bool InitScene();
int messageloop();
void UpdateScene();
void DrawScene();
void CleanUp();


struct Vertex
{
    Vertex(){}
    Vertex(float x, float y, float z)
        :pos(x, y, z) {}
    XMFLOAT3 pos;
    XMFLOAT3 nor;
    XMFLOAT3 col;
};

const D3D11_INPUT_ELEMENT_DESC layout[] =
{
   { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UINT numElements = ARRAYSIZE(layout);

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        i_input.KeyDown((KeyCode)wParam);
        break;
    case WM_KEYUP:
        i_input.KeyUp((KeyCode)wParam);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    if (!InitWinWindow(hInstance,nCmdShow))
    {
        MessageBox(NULL, L"Init WinWindow Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    if (!Initd3dWindow(hInstance))
    {
        MessageBox(NULL, L"Init d3dWindow Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    if (!InitScene())
    {
        MessageBox(NULL, L"Init Scene Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    messageloop();

    CleanUp();
}

bool InitWinWindow(HINSTANCE hInstance, int nCmdShow)
{
    //Step 1: Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        L"The title of my window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, Width, Height,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return true;
}

bool Initd3dWindow(HINSTANCE hInstance)
{
    DXGI_MODE_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

    bufferDesc.Width = Width;
    bufferDesc.Height = Height;
    bufferDesc.RefreshRate.Numerator = 60;
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    DXGI_SWAP_CHAIN_DESC swapChianDesc;

    ZeroMemory(&swapChianDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swapChianDesc.BufferDesc = bufferDesc;
    swapChianDesc.SampleDesc.Count = 1;
    swapChianDesc.SampleDesc.Quality = 0;
    swapChianDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChianDesc.BufferCount = 1;
    swapChianDesc.OutputWindow = hwnd;
    swapChianDesc.Windowed = true;
    swapChianDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL,
        NULL, D3D11_SDK_VERSION, &swapChianDesc, &sp_swapchain, &sp_device, NULL, &sp_context);

    ID3D11Texture2D* BackBuffer;

    sp_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
    sp_device->CreateRenderTargetView(BackBuffer, NULL, &sp_rtv);
    BackBuffer->Release();

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = Width;
    depthStencilDesc.Height = Height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    sp_device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
    sp_device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

    sp_context->OMSetRenderTargets(1, &sp_rtv, depthStencilView);

    return true;
}

bool InitScene()
{
    HRESULT result;
    ID3D10Blob* errorMessage = 0;
    ID3D10Blob* pVSBlob = nullptr;
    ID3D10Blob* pPSBlob = nullptr;
    result = D3DCompileFromFile(L"shadersVS.hlsl", NULL, NULL, "VS", "vs_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &errorMessage);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Compile Vertex Shader Failed", L"Error", MB_OK);
        return 0;
    }
    result = sp_device->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &sp_VertexLayout);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Create input Failed", L"Error", MB_OK);
        return 0;
    }
    result = D3DCompileFromFile(L"shadersPS.hlsl", NULL, NULL, "PS", "ps_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPSBlob, &errorMessage);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Compile pixel Shader Failed", L"Error", MB_OK);
        return 0;
    }

    /*
    //2
    result = CreateShaderFromFile(L"shadersVS.hlsl.cso", L"shadersVS.hlsl", "VS", "vs_5_0", &pVSBlob);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Create Vertex Shader From File Failed", L"Error", MB_OK);
        return 0;
    }
    result = CreateShaderFromFile(L"shadersPS.cso", L"shadersPS.hlsl", "PS", "ps_5_0", &pPSBlob);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Create Pixel Shader From File Failed", L"Error", MB_OK);
        return 0;
    }
    //3
    result = CompileShaderFromFile(L"shadersVS.hlsl", "VS", "vs_4_0", &pVSBlob);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Compile Vertex Shader From File Failed", L"Error", MB_OK);
        return 0;
    }
    result = CompileShaderFromFile(L"shadersPS.hlsl", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Compile Pixel Shader From File Failed", L"Error", MB_OK);
        return 0;
    }*/


    result = sp_device->CreateVertexShader(pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), NULL, &VS);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Create Vertex Shader Failed", L"Error", MB_OK);
        return 0;
    }
    result = sp_device->CreatePixelShader(pPSBlob->GetBufferPointer(),
        pPSBlob->GetBufferSize(), NULL, &PS);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Create Pixel Shader Failed", L"Error", MB_OK);
        return 0;
    }
    GeometryGenerator geoGen;
    
    GeometryGenerator::MeshData box1 = geoGen.CreateBox(0.0f, 0.0f, 0.0f, .3f, .3f, .3f);
  

     UINT16 const vsize = box1.Vertices.size();
    const UINT16 isize = box1.Indices.size();
    {
        Vertex v[24];
        for (int i = 0; i < 24; i++)
        {
            v[i].pos = box1.Vertices[i].Position;
            v[i].nor = box1.Vertices[i].Normal;
            v[i].col = box1.Vertices[i].Color;
        }
        DWORD indices[36];
        for (int i = 0; i < 36; i++)
        {
            indices[i] = box1.Indices[i];
        }
        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(DWORD) * 36;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA idxData;
        idxData.pSysMem = indices;
        sp_device->CreateBuffer(&indexBufferDesc, &idxData, &IndexBuffer);
        sp_context->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        vertexBufferDesc.ByteWidth = sizeof v;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferData;

        ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
        vertexBufferData.pSysMem = v;
        hr = sp_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &triangleVertBuffer);
    }
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    sp_context->IASetVertexBuffers(0, 1, &triangleVertBuffer, &stride, &offset);




    sp_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    sp_context->IASetInputLayout(sp_VertexLayout.Get());

    sp_context->VSSetShader(VS, 0, 0);
    sp_context->PSSetShader(PS, 0, 0);
    D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = Width;
    viewport.Height = Height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    sp_context->RSSetViewports(1, &viewport);

    D3D11_BUFFER_DESC cbbd;
    ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
    cbbd.Usage = D3D11_USAGE_DEFAULT;
    cbbd.ByteWidth = sizeof(TransformCBuffer);
    cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbbd.CPUAccessFlags = 0;
    cbbd.MiscFlags = 0;

    result = sp_device->CreateBuffer(&cbbd, NULL, &simpleCBuffer);
    camera.SetPosition(0.1f, 0.2f, -.5f);
    camera.SetRotation(0.2f, 0.3f, 0.0f);
    camera.GetProjMatrix();



    
    return true;
}
void UpdateScene()
{

    if (i_input.IsKeyDown(0x41))
    {
        XMFLOAT3 pp = camera.GetPosition();
        camera.SetPosition(pp.x, pp.y, pp.z + 0.01f);
        camera.GetProjMatrix();
    }
    if (i_input.IsKeyDown(0x44))
    {
        XMFLOAT3 pp = camera.GetPosition();
        camera.SetPosition(pp.x, pp.y, pp.z - 0.01f);
        camera.GetProjMatrix();
    }

   red += colormodr * 0.0005f;
    green += colormodg * 0.0002f;
    blue += colormodeb * 0.0001;
    if (red >= 1.0f || red <= 0.0f)
        colormodr *= -1;
    if (green >= 1.0f || green <= 0.0f)
        colormodg *= -1;
    if (blue >= 1.0f || blue <= 0.0f)
        colormodeb *= -1;
}

void DrawScene()
{
    const float bgColor[] = {red, green, blue, 1.0f};
    sp_context->ClearRenderTargetView(sp_rtv, bgColor);
    sp_context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f,0.0f);
    sp_context->DrawIndexed(36, 0,0);

    tcbuffer.MVP = XMMatrixIdentity() * camera.GetViewMatrix() * camera.GetProjMatrix();
    tcbuffer.MVP = XMMatrixTranspose(tcbuffer.MVP);
    sp_context->UpdateSubresource(simpleCBuffer, 0, NULL, &tcbuffer, 0, 0);
    sp_context->VSSetConstantBuffers(0, 1, &simpleCBuffer);

    sp_swapchain->Present(0, 0);
}

int messageloop()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (true)
    {
        BOOL PeekMessageL(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin,
            UINT wMsgFilterMax, UINT wRemoveMsg);
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            UpdateScene();
            DrawScene();
        }
    }
    return msg.wParam;
}

void CleanUp()
{
    sp_swapchain->Release();
    sp_device->Release();
    sp_context->Release();
    sp_rtv->Release();

    triangleVertBuffer->Release();
    VS->Release();
    PS->Release();
}

HRESULT CreateShaderFromFile(
    const WCHAR* csoFileNameInOut,
    const WCHAR* hlslFileName,
    LPCSTR entryPoint,
    LPCSTR shaderModel,
    ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    // 寻找是否有已经编译好的顶点着色器
    if (csoFileNameInOut && D3DReadFileToBlob(csoFileNameInOut, ppBlobOut) == S_OK)
    {
        return hr;
    }
    else
    {
        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
        // 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，
        // 但仍然允许着色器进行优化操作
        dwShaderFlags |= D3DCOMPILE_DEBUG;

        // 在Debug环境下禁用优化以避免出现一些不合理的情况
        dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
        ID3DBlob* errorBlob = nullptr;
        hr = D3DCompileFromFile(hlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel,
            dwShaderFlags, 0, ppBlobOut, &errorBlob);
        if (FAILED(hr))
        {
            if (errorBlob != nullptr)
            {
                OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
            }
            SAFE_RELEASE(errorBlob);
            return hr;
        }

        // 若指定了输出文件名，则将着色器二进制信息输出
        if (csoFileNameInOut)
        {
            return D3DWriteBlobToFile(*ppBlobOut, csoFileNameInOut, FALSE);
        }
    }

    return hr;
}

HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    //此处要加载d3dcompiler.lib库
    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
            pErrorBlob->Release();
        }
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}