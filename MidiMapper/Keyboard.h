#pragma once
#include <windows.h>
#include "Shortcut.h"

namespace Keyboard {

    inline DWORD GetExtendedKeyFlag(WORD key) {
        switch (key) {
            case VK_RCONTROL:
            case VK_RMENU:
            case VK_LWIN:
            case VK_RWIN:
            case VK_INSERT:
            case VK_DELETE:
            case VK_HOME:
            case VK_END:
            case VK_PRIOR:
            case VK_NEXT:
            case VK_LEFT:
            case VK_RIGHT:
            case VK_UP:
            case VK_DOWN:
            case VK_DIVIDE:
                return KEYEVENTF_EXTENDEDKEY;
            default:
                return 0;
        }
    }

    void Push(WORD key) {
        INPUT input = {};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
        input.ki.dwFlags = KEYEVENTF_SCANCODE | GetExtendedKeyFlag(key);
        SendInput(1, &input, sizeof(INPUT));
    }

    void Release(WORD key) {
        INPUT input = {};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
        input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP | GetExtendedKeyFlag(key);
        SendInput(1, &input, sizeof(INPUT));
    }

    void Key(Shortcut& shortcut) {
        if (shortcut.ctrl) Push(VK_CONTROL);
        if (shortcut.shift) Push(VK_SHIFT);
        if (shortcut.alt) Push(VK_MENU);
        if (shortcut.win) Push(VK_LWIN);

        WORD key = static_cast<WORD>(shortcut.vKey);
        Push(key);
        Release(key);

        if (shortcut.ctrl) Release(VK_CONTROL);
        if (shortcut.shift) Release(VK_SHIFT);
        if (shortcut.alt) Release(VK_MENU);
        if (shortcut.win) Release(VK_LWIN);
    }
}