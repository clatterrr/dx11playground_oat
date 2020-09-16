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
ID3D11RasterizerState* WireFrame;
ComPtr<ID3D11SamplerState> sp_SamplerState;
ComPtr<ID3D11ShaderResourceView> m_ATex;

//10-light
struct PSConstantStruct
{
    
    Light::PointLight pl;
} cbPerPixel;
ID3D11Buffer* cbPixelCBuffer;
Light TempLight;
UINT totalIndices;

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

struct Vertex
{
    Vertex(){}
    Vertex(float x, float y, float z)
        :pos(x, y, z) {}
    XMFLOAT3 pos;
    XMFLOAT3 nor;
    XMFLOAT3 col;
    XMFLOAT2 uv;
};

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
  //  if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
    //    return true;
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
    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

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
    
    //GeometryGenerator::MeshData box1 = geoGen.CreateBox(0.0f, 0.0f, 0.0f, .3f, .3f, .3f);
  
    GeometryGenerator::MeshData box1 = geoGen.CreateSphere(0.0f, 0.0f, 0.0f, .2f, 10, 10);
     UINT16  vsize = box1.Vertices.size();
    UINT16 isize = box1.Indices.size();
    {
        std::vector<Vertex> v;
        v.resize(vsize);
        for (int i = 0; i < vsize; i++)
        {
            v[i].pos = box1.Vertices[i].Position;
            v[i].nor = box1.Vertices[i].Normal;
            v[i].col = box1.Vertices[i].Color;
            v[i].uv = box1.Vertices[i].UV;
        }
        std::vector<UINT> indices;
        indices.resize(isize);
        for (int i = 0; i < isize; i++)
        {
            indices[i] = box1.Indices[i];
        }
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

    result = CreateWICTextureFromFile(sp_device, L"tex.png", nullptr, m_ATex.GetAddressOf());
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
    
   red = i_input.MouseDeltaX();
   printf("resd = %d,",red);
  
}

void DrawScene()
{
    const float bgColor[] = {red, green, blue, 1.0f};
    sp_context->ClearRenderTargetView(sp_rtv, bgColor);
    sp_context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f,0.0f);


    tcbuffer.MVP = XMMatrixIdentity() * camera.GetViewMatrix() * camera.GetProjMatrix();
    tcbuffer.MVP = XMMatrixTranspose(tcbuffer.MVP);
    sp_context->UpdateSubresource(simpleCBuffer, 0, NULL, &tcbuffer, 0, 0);
    sp_context->VSSetConstantBuffers(0, 1, &simpleCBuffer);
    sp_context->PSSetSamplers(0, 1, sp_SamplerState.GetAddressOf());
    sp_context->PSSetShaderResources(0, 1, m_ATex.GetAddressOf());
    cbPerPixel.pl = TempLight.CreatePointLight();
    //cbPerPixel.mul = 2.0f;
    sp_context->UpdateSubresource(cbPixelCBuffer, 0, NULL, &cbPerPixel,0,0);
    sp_context->PSSetConstantBuffers(0, 1, &cbPixelCBuffer);

    sp_context->VSSetShader(VS, 0, 0);
    sp_context->PSSetShader(PS, 0, 0);

    sp_context->DrawIndexed(totalIndices, 0, 0);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::Begin("Another Window", &show_demo_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");

    ImGui::Text("counter = %f", mousexpos);
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

