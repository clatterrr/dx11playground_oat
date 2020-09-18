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
    XMMATRIX gWorld;
    XMMATRIX gView;
    XMMATRIX gProj;
    XMFLOAT4 gEyePos;
} tcbuffer;

//09-Input
Input i_input;
ID3D11RasterizerState* WireFrame;
ComPtr<ID3D11SamplerState> sp_SamplerState;
std::vector<ComPtr<ID3D11ShaderResourceView>> m_ATex;
ComPtr<ID3D11DepthStencilState> DSSLessEqual;


//10-light
struct PSConstantStruct
{
    
    Light::PointLight pl;
} cbPerPixel;
ID3D11Buffer* cbPixelCBuffer;
Light TempLight;
UINT totalIndices;

//11-cubemap -- failed ,wired skybox

std::vector<GeometryGenerator::MeshData> meshs;
ComPtr<ID3D11ShaderResourceView> skyCubeSRV;
ID3D11VertexShader* skyCubeVS;
ID3D11PixelShader* skyCubePS;
ComPtr<ID3D11RasterizerState> RSNoCull;

//12- render to texture
ID3D11Texture2D* renderTargetTextureMap;
ID3D11RenderTargetView* renderTargetViewMap;
ID3D11ShaderResourceView* shaderResourceViewMap;
ID3D11DepthStencilView* newdepthStencilView;
ID3D11Texture2D* newdepthStencilBuffer;
// Our map camera's view and projection matrices
XMMATRIX mapView;
XMMATRIX mapProjection;
D3D11_VIEWPORT                                 viewport;
D3D11_VIEWPORT						m_OutputViewPort = {};	// 输出所用的视口

UINT vsize = 0, isize = 0, vk = 0, vi = 0;

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
float mousexpos = 0.0f;



const D3D11_INPUT_ELEMENT_DESC layout[] =
{
   { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,36,D3D11_INPUT_PER_VERTEX_DATA,0}
};
UINT numElements = ARRAYSIZE(layout);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;
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
    case WM_INPUT:
    {
        UINT rawInputSize = 48;
        LPBYTE inputBuffer[48];
        ZeroMemory(inputBuffer, rawInputSize);

        GetRawInputData(
            (HRAWINPUT)lParam,
            RID_INPUT,
            inputBuffer,
            &rawInputSize,
            sizeof(RAWINPUTHEADER));

        RAWINPUT* raw = (RAWINPUT*)inputBuffer;

        if (raw->header.dwType == RIM_TYPEMOUSE &&
            raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
        {
            long xPosRelative = raw->data.mouse.lLastX;
            long yPosrelative = raw->data.mouse.lLastY;
            mousexpos = xPosRelative;
            i_input.UpdateMousePos(xPosRelative, yPosrelative);
            SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        }
        break;
    }
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


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(sp_device, sp_context);
    if (!InitScene())
    {
        MessageBox(NULL, L"Init Scene Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x02;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = hwnd;
   // RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

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
    sp_device->CreateTexture2D(&depthStencilDesc, NULL, &newdepthStencilBuffer);
    sp_device->CreateDepthStencilView(newdepthStencilBuffer, NULL, &newdepthStencilView);
    sp_context->OMSetRenderTargets(1, &sp_rtv, depthStencilView);



    return true;
}

bool InitScene()
{
    HRESULT result;
    ID3D10Blob* errorMessage = 0;
    ID3D10Blob* pVSBlob = nullptr;
    ID3D10Blob* pPSBlob = nullptr;
    result = D3DCompileFromFile(L"shadersVS.hlsl", NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &errorMessage);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Compile Vertex Shader Failed", L"Error", MB_OK);
        return 0;
    }
    result = sp_device->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &sp_VertexLayout);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Create input Failed", L"Error", MB_OK);
        return 0;
    }
    result = D3DCompileFromFile(L"shadersPS.hlsl", NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPSBlob, &errorMessage);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Compile pixel Shader Failed", L"Error", MB_OK);
        return 0;
    }

    result = sp_device->CreateVertexShader(pVSBlob->GetBufferPointer(),pVSBlob->GetBufferSize(), NULL, &VS);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Create Vertex Shader Failed", L"Error", MB_OK);
        return 0;
    }
    result = sp_device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &PS);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Create Pixel Shader Failed", L"Error", MB_OK);
        return 0;
    }

    // skycubehsls
    result = D3DCompileFromFile(L"skycubeVS.hlsl", NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &errorMessage);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Compile Vertex Shader Failed", L"Error", MB_OK);
        return 0;
    }
    result = D3DCompileFromFile(L"skycubePS.hlsl", NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPSBlob, &errorMessage);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Compile pixel Shader Failed", L"Error", MB_OK);
        return 0;
    }
    result = sp_device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &skyCubeVS);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Create Vertex Shader Failed", L"Error", MB_OK);
        return 0;
    }
    result = sp_device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &skyCubePS);
    if (FAILED(result))
    {
        pVSBlob->Release();
        MessageBox(nullptr,
            L"Create Pixel Shader Failed", L"Error", MB_OK);
        return 0;
    }

    GeometryGenerator geoGen;
    meshs.empty();

    meshs.push_back(geoGen.CreateBox(0.0f, 0.0f, 1.0f, .2f, .2f, .2f));
    meshs.push_back(geoGen.CreateBox(0.5f, 0.0f, 1.0f, .2f, .2f, .2f));
    meshs.push_back(geoGen.CreateSphere(0.25f, 0.0f, 1.0f, 0.15f, 15, 15));
    meshs.push_back(geoGen.CreateSphere(0.0f, 0.0f, 0.0f, 0.5f, 15, 15));
  //  meshs.emplace_back(geoGen.CreateSphere(0.0f, 0.0f, 0.0f, .2f, 10, 10));

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
        totalIndices = isize;
        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(UINT) * isize;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA idxData;
        idxData.pSysMem = &indices[0];
        sp_device->CreateBuffer(&indexBufferDesc, &idxData, &IndexBuffer);
        sp_context->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

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
        hr = sp_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &triangleVertBuffer);
    }
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    sp_context->IASetVertexBuffers(0, 1, &triangleVertBuffer, &stride, &offset);




    sp_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    sp_context->IASetInputLayout(sp_VertexLayout.Get());

    sp_context->VSSetShader(VS, 0, 0);
    sp_context->PSSetShader(PS, 0, 0);
  
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = Width;
    viewport.Height = Height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

   
    
    D3D11_BUFFER_DESC cbbd;
    ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
    cbbd.Usage = D3D11_USAGE_DEFAULT;
    cbbd.ByteWidth = sizeof(TransformCBuffer);
    cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbbd.CPUAccessFlags = 0;
    cbbd.MiscFlags = 0;

    result = sp_device->CreateBuffer(&cbbd, NULL, &simpleCBuffer);
    
    if (FAILED(result))
    {
        MessageBox(nullptr,
            L"Create SimpleBuffer Failed", L"Error", MB_OK);
        return 0;
    }

    D3D11_BUFFER_DESC ccbbd;
    ZeroMemory(&ccbbd, sizeof(D3D11_BUFFER_DESC));
    ccbbd.Usage = D3D11_USAGE_DEFAULT;
    ccbbd.ByteWidth = sizeof(PSConstantStruct);
    ccbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ccbbd.CPUAccessFlags = 0;
    ccbbd.MiscFlags = 0;

    result = sp_device->CreateBuffer(&ccbbd, NULL, &cbPixelCBuffer);

    if (FAILED(result))
    {
        MessageBox(nullptr,
            L"Create PerPixelBuffer Failed", L"Error", MB_OK);
        return 0;
    }

    camera.SetPosition(0.1f, 0.2f, -.5f);
    camera.SetRotation(0.2f, 0.3f, 0.0f);
    camera.GetProjMatrix();

    D3D11_RASTERIZER_DESC wfdesc;
    ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
    wfdesc.FillMode = D3D11_FILL_WIREFRAME;
    wfdesc.CullMode = D3D11_CULL_NONE;
    result = sp_device->CreateRasterizerState(&wfdesc, &WireFrame);

    //sp_context->RSSetState(WireFrame);是否开启线框
    m_ATex.resize(2);
    result = CreateWICTextureFromFile(sp_device, L"tex.png", nullptr, m_ATex[0].GetAddressOf());
    if (FAILED(result))
    {
        MessageBox(nullptr,
            L"Create WIC Failed", L"Error", MB_OK);
        return 0;
    }
    result = CreateWICTextureFromFile(sp_device, L"tex2.png", nullptr, m_ATex[1].GetAddressOf());
    if (FAILED(result))
    {
        MessageBox(nullptr,
            L"Create WIC Failed", L"Error", MB_OK);
        return 0;
    }

    //SamplerState

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    sp_device->CreateSamplerState(&sampDesc, sp_SamplerState.GetAddressOf());



    D3D11_TEXTURE2D_DESC skyTexDesc = {};
    skyTexDesc.Width = 100;
    skyTexDesc.Height = 100;
    skyTexDesc.MipLevels = 1;
    skyTexDesc.ArraySize = 6;
    skyTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    skyTexDesc.SampleDesc.Count = 1;
    skyTexDesc.SampleDesc.Quality = 0;
    skyTexDesc.Usage = D3D11_USAGE_DEFAULT;
    skyTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    skyTexDesc.CPUAccessFlags = 0;
    skyTexDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    ComPtr<ID3D11Texture2D> skyTextures;
    result = sp_device->CreateTexture2D(&skyTexDesc, nullptr, skyTextures.GetAddressOf());

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = skyTexDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    result = sp_device->CreateShaderResourceView(skyTextures.Get(), &srvDesc, skyCubeSRV.GetAddressOf());
    result = CreateWICTextureFromFile(sp_device,L"sun.png",nullptr,skyCubeSRV.GetAddressOf(),0);

    D3D11_RASTERIZER_DESC rasterizerDesc;
    // 无背面剔除模式
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthClipEnable = true;
    result = sp_device->CreateRasterizerState(&rasterizerDesc, RSNoCull.GetAddressOf());

    D3D11_DEPTH_STENCIL_DESC dsDesc;

    // 允许使用深度值一致的像素进行替换的深度/模板状态
    // 该状态用于绘制天空盒，因为深度值为1.0时默认无法通过深度测试
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

   dsDesc.StencilEnable = false;

    result = sp_device->CreateDepthStencilState(&dsDesc, DSSLessEqual.GetAddressOf());

    // What we will be doing, is first create a 2d texture. We will then use this texture as a render
// target AND a shader resource. We cannot render to a shader resource directly, so to do this,
// we will create a render target and shader resource separately as pointers to this texture. Then
// when we want to render to the texture, we will render to the render target, which is actually
// a pointer to the texture, so we will be rendering to the texture. When using the shader resource,
// we are actually getting the data from the texture that the shader resource points to
    D3D11_TEXTURE2D_DESC textureDesc;
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

    ///////////////////////// Map's Texture
    // Initialize the  texture description.
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    // Setup the texture description.
    // We will have our map be a square
    // We will need to have this texture bound as a render target AND a shader resource
    textureDesc.Width = Width / 2;
    textureDesc.Height = Height / 2;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    // Create the texture
    sp_device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureMap);

    /////////////////////// Map's Render Target
    // Setup the description of the render target view.
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    // Create the render target view.
    sp_device->CreateRenderTargetView(renderTargetTextureMap, &renderTargetViewDesc, &renderTargetViewMap);

    // Setup the description of the shader resource view.
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    // Create the shader resource view.
    sp_device->CreateShaderResourceView(renderTargetTextureMap, &shaderResourceViewDesc, &shaderResourceViewMap);

    //////////////////////// Map's camera information
    // We will have the camera follow the player
    XMVECTOR mapCamPosition = XMVectorSet(camera.GetPosition().x, camera.GetPosition().y + 1.0f, camera.GetPosition().z,1.0f);
    XMVECTOR mapCamTarget = XMLoadFloat3(&camera.GetPosition());
    XMVECTOR mapCamUp = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    //Set the View matrix
    mapView = XMMatrixLookAtLH(mapCamPosition, mapCamTarget, mapCamUp);

    // Build an orthographic projection matrix
    mapProjection = XMMatrixOrthographicLH(512, 512, 1.0f, 1000.0f);

    m_OutputViewPort.TopLeftX = 0.0f;
    m_OutputViewPort.TopLeftY = 0.0f;
    m_OutputViewPort.Width = static_cast<float>(200);
    m_OutputViewPort.Height = static_cast<float>(200);
    m_OutputViewPort.MinDepth = 0.0f;
    m_OutputViewPort.MaxDepth = 1.0f;

    return true;
}
void UpdateScene()
{
    if (i_input.IsKeyDown(0x1B))
    {
        DestroyWindow(hwnd);
    }

    if (i_input.IsKeyDown(0x57))
    {
        XMFLOAT3 pp = camera.GetPosition();
        camera.SetPosition(pp.x, pp.y, pp.z + 0.001f);
        camera.GetProjMatrix();
    }
    if (i_input.IsKeyDown(0x53))
    {
        XMFLOAT3 pp = camera.GetPosition();
        camera.SetPosition(pp.x, pp.y, pp.z - 0.001f);
        camera.GetProjMatrix();
    }
       // XMFLOAT3 rr = camera.GetRotation();
      //  camera.SetRotation(rr.x + i_input.MouseDeltaX()*0.00005f,rr.y,rr.z);
        camera.GetProjMatrix();
 /*   if (i_input.MouseDeltaX() < 0)
    {
        XMFLOAT3 rr = camera.GetRotation();
        camera.SetRotation(rr.x - 0.001f, rr.y, rr.z);
        camera.GetProjMatrix();
    }*/
    
   red = i_input.MouseDeltaX();
   printf("resd = %d,",red);
  
}

void DrawScene()
{
    const float bgColor[] = {red, green, blue, 1.0f};
  
  


    tcbuffer.MVP = XMMatrixIdentity() * camera.GetViewMatrix() * camera.GetProjMatrix();
    tcbuffer.MVP = XMMatrixTranspose(tcbuffer.MVP);
    XMVECTOR rotationVec = XMLoadFloat3(&camera.GetRotation());
    XMFLOAT3 xm = XMFLOAT3(-0.25f, 0.0f, 0.0f);
   XMVECTOR positionVec = XMLoadFloat3(&xm);
//    XMVECTOR positionVec = XMLoadFloat3(&camera.GetPosition());
    XMMATRIX World = XMMatrixTranslationFromVector(positionVec);
    tcbuffer.gWorld = XMMatrixTranspose(World);
    tcbuffer.gView = camera.GetViewMatrix();
    tcbuffer.gProj = camera.GetProjMatrix();
    tcbuffer.gEyePos = XMFLOAT4(camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z,0.0f);
    sp_context->UpdateSubresource(simpleCBuffer, 0, NULL, &tcbuffer, 0, 0);
    sp_context->VSSetConstantBuffers(0, 1, &simpleCBuffer);
    sp_context->PSSetSamplers(0, 1, sp_SamplerState.GetAddressOf());
    sp_context->PSSetShaderResources(0, 1, m_ATex[0].GetAddressOf());
    cbPerPixel.pl = TempLight.CreatePointLight();
    //cbPerPixel.mul = 2.0f;
    sp_context->UpdateSubresource(cbPixelCBuffer, 0, NULL, &cbPerPixel,0,0);



    sp_context->PSSetConstantBuffers(0, 1, &cbPixelCBuffer);
    sp_context->VSSetShader(VS, 0, 0);
    sp_context->PSSetShader(PS, 0, 0);

  
    //render to anothor texture

    sp_context->ClearRenderTargetView(renderTargetViewMap, bgColor);
    sp_context->ClearDepthStencilView(newdepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
    sp_context->OMSetRenderTargets(1, &renderTargetViewMap, newdepthStencilView);
    sp_context->RSSetViewports(1, &m_OutputViewPort);
    sp_context->DrawIndexed(totalIndices - meshs[meshs.size() - 1].Indices.size(), 0, 0);
    //why viewport disappeared


    sp_context->ClearRenderTargetView(sp_rtv, bgColor);
   sp_context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
    sp_context->OMSetRenderTargets(1, &sp_rtv, depthStencilView);
    sp_context->RSSetViewports(1, &viewport);
    sp_context->PSSetShaderResources(0, 1, &shaderResourceViewMap);
    sp_context->DrawIndexed(totalIndices - meshs[meshs.size() - 1].Indices.size(), 0, 0);





   // sp_context->PSSetShaderResources(0, 1, m_ATex[1].GetAddressOf());
    //sp_context->DrawIndexed(36, 36, 0);

  //  sp_context->RSSetState(RSNoCull.Get());
    sp_context->OMSetDepthStencilState(DSSLessEqual.Get(), 0);
    sp_context->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    sp_context->VSSetShader(skyCubeVS, 0, 0);
    sp_context->PSSetShader(skyCubePS, 0, 0);
    sp_context->PSSetShaderResources(0, 1, skyCubeSRV.GetAddressOf());
    sp_context->DrawIndexed(meshs[meshs.size() - 1].Indices.size(), totalIndices - meshs[meshs.size() - 1].Indices.size(), 0);


    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::Begin("Another Window", &show_demo_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    ImGui::SliderAngle("yaw", &camera.c_rotation.x, -90.0f, 90.0f);
    ImGui::Text("vsize = %d",vsize);
    ImGui::Text("isize = %d", isize);
    ImGui::Text("vk = %d", vk);
    ImGui::Text("totalIndices = %d", totalIndices);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

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
    cbPixelCBuffer->Release();
}

