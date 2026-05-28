#pragma once

#include <d3d11.h>

namespace DXManager {
    extern ID3D11Device* device;
    extern ID3D11DeviceContext* deviceContext;
    extern IDXGISwapChain* swapChain;
    extern ID3D11RenderTargetView* rendererTargetView;

    void CreateRenderTarget();
    bool Create(HWND hwnd);
    void Resize(WPARAM wParam, LPARAM lParam);
    void Cleanup();

    template<typename T>
    void CleanupItem(T*& item);
}