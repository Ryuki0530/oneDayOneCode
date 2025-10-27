# DirectX 11 基礎ドキュメント
## ― 背景色を描画するための基礎構造 ―

---

## 1. DirectX とは
DirectX は Microsoft が提供するマルチメディア API で、
ゲームや 3D グラフィックスの描画、オーディオ再生などを行うためのライブラリ群です。

今回扱う **Direct3D 11 (D3D11)** は 3D/2D グラフィックス描画を担うモジュールです。

---

## 2. 必要なヘッダとライブラリ

```cpp
#include <Windows.h>
#include <d3d11.h>           // Direct3D 11 メインヘッダ
#pragma comment(lib, "d3d11.lib")
```

### ✅ ポイント
- `Windows.h` … ウィンドウ作成・メッセージ処理用  
- `d3d11.h` … Direct3D 11 のクラスと関数群  
- `#pragma comment(lib, "d3d11.lib")` … d3d11 ライブラリをリンク

---

## 3. Direct3D 11 の基本構成

Direct3D では、主に以下のオブジェクトを使用します。

| オブジェクト名 | 役割 |
|----------------|------|
| **Device (`ID3D11Device`)** | GPU にリソースを作成するインターフェース |
| **Device Context (`ID3D11DeviceContext`)** | GPU に描画命令を送る |
| **Swap Chain (`IDXGISwapChain`)** | バックバッファと前面バッファを切り替える |
| **Render Target View (`ID3D11RenderTargetView`)** | 描画対象を指定するビュー |

---

## 4. 初期化の流れ

1. **ウィンドウを作成**
   - Win32 API (`CreateWindowEx`) を用いる。

2. **Direct3D デバイスの作成**

```cpp
D3D_FEATURE_LEVEL featureLevel;
D3D11CreateDeviceAndSwapChain(
    nullptr,                    // デフォルトアダプタ
    D3D_DRIVER_TYPE_HARDWARE,   // GPUを使用
    nullptr,                    // ソフトウェアDLLなし
    0,                          // フラグ
    nullptr, 0,                 // フィーチャーレベル自動選択
    D3D11_SDK_VERSION,
    &swapChainDesc,             // スワップチェーン設定
    &swapChain,                 // 出力スワップチェーン
    &device,                    // 出力デバイス
    &featureLevel,              // 実際に選ばれたフィーチャーレベル
    &deviceContext              // 出力デバイスコンテキスト
);
```

3. **レンダーターゲットの設定**

```cpp
ID3D11Texture2D* backBuffer;
swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
backBuffer->Release();

deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
```

4. **ビューポートの設定**

```cpp
D3D11_VIEWPORT viewport = {};
viewport.TopLeftX = 0;
viewport.TopLeftY = 0;
viewport.Width = (FLOAT)width;
viewport.Height = (FLOAT)height;
viewport.MinDepth = 0.0f;
viewport.MaxDepth = 1.0f;
deviceContext->RSSetViewports(1, &viewport);
```

---

## 5. 描画ループ（メッセージループ）

```cpp
while (true) {
    // Windowsメッセージ処理
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    } else {
        // 背景色で画面をクリア
        const float clearColor[4] = { 0.0f, 0.2f, 0.6f, 1.0f }; // RGBA
        deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

        // 表示
        swapChain->Present(1, 0);
    }
}
```

---

## 6. 終了処理

```cpp
if (renderTargetView) renderTargetView->Release();
if (swapChain) swapChain->Release();
if (deviceContext) deviceContext->Release();
if (device) device->Release();
```

---

## 7. よくあるエラーと対策

| 症状 | 原因 | 対策 |
|------|------|------|
| `E_INVALIDARG` エラー | `SwapChainDesc` の設定ミス | バッファ数やフォーマットを確認 |
| 黒い画面が出ない | `Present()` が呼ばれていない | 描画ループを確認 |
| コンパイルできない | Windows SDK が見つからない | Visual Studio Build Toolsを再インストール |

---

## 8. 構造図

```
┌────────────────────────────┐
│ WinMain                    │
│  └─ CreateWindow()         │
│  └─ D3D11CreateDevice...() │
│  └─ 描画ループ              │
│        ├─ ClearRenderTargetView()
│        ├─ Present()
│        └─ MessageLoop
└────────────────────────────┘
```

---

## 9. 参考資料

- Microsoft Docs: [Direct3D 11 Programming Guide](https://learn.microsoft.com/en-us/windows/win32/direct3d11/)
- GitHub: [DirectX-SDK-Samples](https://github.com/microsoft/DirectX-SDK-Samples)
- 書籍: 『DirectX 11 3D プログラミング入門』（オライリー・ジャパン）

---

## 10. 次のステップ

- 背景色をアニメーションさせる
- 三角形を描画する
- シェーダー (HLSL) を導入する
