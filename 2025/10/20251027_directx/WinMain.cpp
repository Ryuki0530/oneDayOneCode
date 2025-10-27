#define UNICODE
#define _UNICODE

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// グローバル変数
HWND g_hWnd = nullptr;
ID3D11Device* g_Device = nullptr;
ID3D11DeviceContext* g_Context = nullptr;
IDXGISwapChain* g_SwapChain = nullptr;
ID3D11RenderTargetView* g_RTV = nullptr;

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_ERASEBKGND:
        // GDIによる背景の白塗りを抑制
        return 1;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

// Direct3D 初期化
bool InitD3D(HWND hWnd, int width, int height)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        &g_SwapChain,
        &g_Device,
        nullptr,
        &g_Context
    );

    if (FAILED(hr)) {
        MessageBoxA(nullptr, "D3D11CreateDeviceAndSwapChain failed", "Error", MB_OK);
        return false;
    }

    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) {
        MessageBoxA(nullptr, "GetBuffer failed", "Error", MB_OK);
        return false;
    }

    hr = g_Device->CreateRenderTargetView(pBackBuffer, nullptr, &g_RTV);
    pBackBuffer->Release();
    if (FAILED(hr)) {
        MessageBoxA(nullptr, "CreateRenderTargetView failed", "Error", MB_OK);
        return false;
    }

    // レンダーターゲット設定
    g_Context->OMSetRenderTargets(1, &g_RTV, nullptr);

    // ビューポート設定
    D3D11_VIEWPORT vp = {};
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    g_Context->RSSetViewports(1, &vp);

    return true;
}

// 描画
void Render()
{
    const float clearColor[4] = { 0.0f, 0.2f, 0.6f, 1.0f }; // RGBA（濃い青）
    g_Context->ClearRenderTargetView(g_RTV, clearColor);
    g_SwapChain->Present(0, 0);
}

// 終了処理
void Cleanup()
{
    if (g_RTV) g_RTV->Release();
    if (g_SwapChain) g_SwapChain->Release();
    if (g_Context) g_Context->Release();
    if (g_Device) g_Device->Release();
}

// エントリポイント
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    const int WIDTH = 800, HEIGHT = 600;

    // ウィンドウクラス登録
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"DX11WindowClass";
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // DC固定
    wc.hbrBackground = nullptr;                    // GDI背景無効
    RegisterClass(&wc);

    // ウィンドウ作成
    g_hWnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        L"DirectX11 Blue Background",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WIDTH, HEIGHT,
        nullptr, nullptr, hInst, nullptr
    );

    ShowWindow(g_hWnd, SW_SHOW);

    // D3D初期化
    if (!InitD3D(g_hWnd, WIDTH, HEIGHT))
        return -1;

    // メッセージループ
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Render();
        }
    }

    Cleanup();
    return 0;
}
