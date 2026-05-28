#pragma once
#include <windows.h>

extern HWND hwnd;

void TrayIcon(HWND hwnd);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);