#include "MidiMapper.h"
#include <iostream>
#include <windows.h>
#include <d3d11.h>
#include "DXManager.h"
#include "IGManager.h"
#include "MidiController.h"
#include "ConfigManager.h"

#pragma comment(lib, "d3d11.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hwnd = NULL;
bool running = true;
NOTIFYICONDATAA iconData = {};
#define WM_TRAYICON (WM_USER + 1)
const wchar_t NAME[] = L"MidiMapper";

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = NAME;
    RegisterClassW(&wc);

    hwnd = CreateWindowExW(WS_EX_TOOLWINDOW, NAME, NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL)
        return 0;

    if (!DXManager::Create(hwnd))
        return 1;

    ConfigManager::Load();
    IGManager::Create(hwnd);
    MidiController::Init();
    TrayIcon(hwnd);
    ShowWindow(hwnd, SW_HIDE);

    MSG msg = {};
    while (running)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                running = false;
        }

        if (IsWindowVisible(hwnd))
        {
            IGManager::Render();
        }
        else
        {
            Sleep(10);
        }
    }

    Shell_NotifyIconA(NIM_DELETE, &iconData);
    IGManager::Destroy();
    MidiController::Close();
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg)
    {
        case WM_SIZE:
            DXManager::Resize(wParam, lParam);
            return 0;
        case WM_CLOSE:
            ShowWindow(hwnd, SW_HIDE);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_TRAYICON:
        {
            if (lParam == WM_LBUTTONUP)
            {
                SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 1280, 720, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

                ShowWindow(hwnd, SW_SHOW);
                SetForegroundWindow(hwnd);
            }
            if (lParam == WM_RBUTTONUP)
            {
                running = false;
                PostQuitMessage(0);
            }
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void TrayIcon(HWND hwnd)
{
    iconData.cbSize = sizeof(NOTIFYICONDATAA);
    iconData.hWnd = hwnd;
    iconData.uID = 1;
    iconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    iconData.uCallbackMessage = WM_TRAYICON;
    iconData.hIcon = (HICON)LoadImageA(NULL, "icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);

    strcpy_s(iconData.szTip, "MidiMapper");

    Shell_NotifyIconA(NIM_ADD, &iconData);
}