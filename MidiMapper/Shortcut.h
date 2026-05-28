#pragma once
#include <Windows.h>
#include <imgui.h>
#include <nlohmann/json.hpp>

class Shortcut
{
public:
    bool ctrl = false;
    bool shift = false;
    bool alt = false;
    bool win = false;
    int imKey = -1, vKey = 1;

    Shortcut() {}

    void SetKey(int key) {
        imKey = key;
        if (key == -1) {
            vKey = -1;
            return;
        }

        switch (key)
        {
            case ImGuiKey_A: vKey = 'A'; break;
            case ImGuiKey_B: vKey = 'B'; break;
            case ImGuiKey_C: vKey = 'C'; break;
            case ImGuiKey_D: vKey = 'D'; break;
            case ImGuiKey_E: vKey = 'E'; break;
            case ImGuiKey_F: vKey = 'F'; break;
            case ImGuiKey_G: vKey = 'G'; break;
            case ImGuiKey_H: vKey = 'H'; break;
            case ImGuiKey_I: vKey = 'I'; break;
            case ImGuiKey_J: vKey = 'J'; break;
            case ImGuiKey_K: vKey = 'K'; break;
            case ImGuiKey_L: vKey = 'L'; break;
            case ImGuiKey_M: vKey = 'M'; break;
            case ImGuiKey_N: vKey = 'N'; break;
            case ImGuiKey_O: vKey = 'O'; break;
            case ImGuiKey_P: vKey = 'P'; break;
            case ImGuiKey_Q: vKey = 'Q'; break;
            case ImGuiKey_R: vKey = 'R'; break;
            case ImGuiKey_S: vKey = 'S'; break;
            case ImGuiKey_T: vKey = 'T'; break;
            case ImGuiKey_U: vKey = 'U'; break;
            case ImGuiKey_V: vKey = 'V'; break;
            case ImGuiKey_W: vKey = 'W'; break;
            case ImGuiKey_X: vKey = 'X'; break;
            case ImGuiKey_Y: vKey = 'Y'; break;
            case ImGuiKey_Z: vKey = 'Z'; break;

            case ImGuiKey_0: vKey = '0'; break;
            case ImGuiKey_1: vKey = '1'; break;
            case ImGuiKey_2: vKey = '2'; break;
            case ImGuiKey_3: vKey = '3'; break;
            case ImGuiKey_4: vKey = '4'; break;
            case ImGuiKey_5: vKey = '5'; break;
            case ImGuiKey_6: vKey = '6'; break;
            case ImGuiKey_7: vKey = '7'; break;
            case ImGuiKey_8: vKey = '8'; break;
            case ImGuiKey_9: vKey = '9'; break;

            case ImGuiKey_F1: vKey = VK_F1; break;
            case ImGuiKey_F2: vKey = VK_F2; break;
            case ImGuiKey_F3: vKey = VK_F3; break;
            case ImGuiKey_F4: vKey = VK_F4; break;
            case ImGuiKey_F5: vKey = VK_F5; break;
            case ImGuiKey_F6: vKey = VK_F6; break;
            case ImGuiKey_F7: vKey = VK_F7; break;
            case ImGuiKey_F8: vKey = VK_F8; break;
            case ImGuiKey_F9: vKey = VK_F9; break;
            case ImGuiKey_F10: vKey = VK_F10; break;
            case ImGuiKey_F11: vKey = VK_F11; break;
            case ImGuiKey_F12: vKey = VK_F12; break;

            case ImGuiKey_Enter: vKey = VK_RETURN; break;
            case ImGuiKey_Tab: vKey = VK_TAB; break;
            case ImGuiKey_Backspace: vKey = VK_BACK; break;
            case ImGuiKey_Space: vKey = VK_SPACE; break;
            case ImGuiKey_Escape: vKey = VK_ESCAPE; break;
            case ImGuiKey_Insert: vKey = VK_INSERT; break;
            case ImGuiKey_Delete: vKey = VK_DELETE; break;
            case ImGuiKey_Home: vKey = VK_HOME; break;
            case ImGuiKey_End: vKey = VK_END; break;
            case ImGuiKey_PageUp: vKey = VK_PRIOR; break;
            case ImGuiKey_PageDown: vKey = VK_NEXT; break;
            case ImGuiKey_CapsLock: vKey = VK_CAPITAL; break;
            case ImGuiKey_PrintScreen: vKey = VK_SNAPSHOT; break;
            case ImGuiKey_Pause: vKey = VK_PAUSE; break;
            case ImGuiKey_Menu: vKey = VK_APPS; break;

            case ImGuiKey_LeftArrow: vKey = VK_LEFT; break;
            case ImGuiKey_RightArrow: vKey = VK_RIGHT; break;
            case ImGuiKey_UpArrow: vKey = VK_UP; break;
            case ImGuiKey_DownArrow: vKey = VK_DOWN; break;

            case ImGuiKey_GraveAccent: vKey = VK_OEM_3; break;
            case ImGuiKey_Minus: vKey = VK_OEM_MINUS; break;
            case ImGuiKey_Equal: vKey = VK_OEM_PLUS; break;
            case ImGuiKey_LeftBracket: vKey = VK_OEM_4; break;
            case ImGuiKey_RightBracket: vKey = VK_OEM_6; break;
            case ImGuiKey_Backslash: vKey = VK_OEM_5; break;
            case ImGuiKey_Semicolon: vKey = VK_OEM_1; break;
            case ImGuiKey_Apostrophe: vKey = VK_OEM_7; break;
            case ImGuiKey_Comma: vKey = VK_OEM_COMMA; break;
            case ImGuiKey_Period: vKey = VK_OEM_PERIOD; break;
            case ImGuiKey_Slash: vKey = VK_OEM_2; break;

            case ImGuiKey_Keypad0: vKey = VK_NUMPAD0; break;
            case ImGuiKey_Keypad1: vKey = VK_NUMPAD1; break;
            case ImGuiKey_Keypad2: vKey = VK_NUMPAD2; break;
            case ImGuiKey_Keypad3: vKey = VK_NUMPAD3; break;
            case ImGuiKey_Keypad4: vKey = VK_NUMPAD4; break;
            case ImGuiKey_Keypad5: vKey = VK_NUMPAD5; break;
            case ImGuiKey_Keypad6: vKey = VK_NUMPAD6; break;
            case ImGuiKey_Keypad7: vKey = VK_NUMPAD7; break;
            case ImGuiKey_Keypad8: vKey = VK_NUMPAD8; break;
            case ImGuiKey_Keypad9: vKey = VK_NUMPAD9; break;

            case ImGuiKey_KeypadDecimal: vKey = VK_DECIMAL; break;
            case ImGuiKey_KeypadDivide: vKey = VK_DIVIDE; break;
            case ImGuiKey_KeypadMultiply: vKey = VK_MULTIPLY; break;
            case ImGuiKey_KeypadSubtract: vKey = VK_SUBTRACT; break;
            case ImGuiKey_KeypadAdd: vKey = VK_ADD; break;
            case ImGuiKey_KeypadEnter: vKey = VK_RETURN; break;
        }
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Shortcut, ctrl, shift, alt, win, imKey, vKey)