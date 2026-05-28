#include "DXManager.h"
#include <windows.h>
#include <d3d11.h>
#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

namespace DXManager {
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
    IDXGISwapChain* swapChain = nullptr;
    ID3D11RenderTargetView* rendererTargetView = nullptr;

	bool Create(HWND hwnd){
        BOOL darkMode = TRUE;
        DwmSetWindowAttribute(hwnd, 20, &darkMode, sizeof(darkMode));

        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 2;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT flags = 0;
        D3D_FEATURE_LEVEL feature;
        const D3D_FEATURE_LEVEL featureLevel[2] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0};

        HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevel, 2, D3D11_SDK_VERSION, &sd, &swapChain, &device, &feature, &deviceContext);
        if (res != S_OK)
            return false;

        CreateRenderTarget();
        return true;
	}

    void CreateRenderTarget() {
        ID3D11Texture2D* texture = nullptr;
        swapChain->GetBuffer(0, IID_PPV_ARGS(&texture));
        if (texture) {
            device->CreateRenderTargetView(texture, nullptr, &rendererTargetView);
            texture->Release();
        }
    }

    void Resize(WPARAM wParam, LPARAM lParam){
        if (device != nullptr && wParam != SIZE_MINIMIZED) {
            CleanupItem(rendererTargetView);
            swapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
    }

    void Cleanup() {
        CleanupItem(rendererTargetView);
        CleanupItem(swapChain);
        CleanupItem(deviceContext);
        CleanupItem(device);
    }

    template<typename T>
    void CleanupItem(T*& item) {
        if (item){
            item->Release();
            item = nullptr;
        }
    }
}