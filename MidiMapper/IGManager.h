#include <Windows.h>
#include "imgui.h"
#include "Shortcut.h"

namespace IGManager {
    extern ImGuiIO* io;
    void Create(HWND hwnd);
    void Render();
    void Frame();
    void LoadStyle();
    bool RecordShortcut(Shortcut& shortcut, ImVec2 size);
    void Destroy();
}