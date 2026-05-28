#define _CRT_SECURE_NO_WARNINGS
#include "MidiMapper.h"
#include "IGManager.h"
#include "DXManager.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <d3d11.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "MidiController.h"
#include "ConfigManager.h"
#include "Shortcut.h"

namespace IGManager{
    ImVec4 background = ImVec4(0.05f, 0.05f, 0.06f, 1.0f);
    ImVec4 cardBackground = ImVec4(0.09f, 0.09f, 0.11f, 1.0f);
    ImVec4 knobBackground = ImVec4(0.14f, 0.14f, 0.17f, 1.0f);
    ImVec4 borderFluent = ImVec4(0.18f, 0.18f, 0.22f, 1.0f);
    ImVec4 textBright = ImVec4(0.96f, 0.96f, 0.98f, 1.0f);
    ImVec4 textDim = ImVec4(0.55f, 0.55f, 0.60f, 1.0f);
    ImVec4 accentNeon = ImVec4(0.00f, 0.90f, 0.65f, 1.0f);
    ImVec4 accentSoft = ImVec4(0.00f, 0.55f, 0.40f, 0.3f);

    ImGuiIO* io = nullptr;
    int selected = 0;
    unsigned long frameCounter = 0;
    Shortcut* activeRecording = nullptr;

	void Create(HWND hwnd){
        ImGui::CreateContext();
        io = &ImGui::GetIO();
        LoadStyle();
        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX11_Init(DXManager::device, DXManager::deviceContext);
	}

    void LoadStyle(){
        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowPadding = ImVec2(0, 0);
        style.FramePadding = ImVec2(16, 12);
        style.ItemSpacing = ImVec2(24, 24);
        style.ItemInnerSpacing = ImVec2(12, 12);

        style.ScrollbarRounding = 12.0f;
        style.WindowBorderSize = 0.0f;
        style.ChildBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;

        ImVec4* c = style.Colors;
        c[ImGuiCol_Text] = textBright;
        c[ImGuiCol_TextDisabled] = textDim;
        c[ImGuiCol_WindowBg] = background;
        c[ImGuiCol_ChildBg] = cardBackground;
        c[ImGuiCol_Border] = borderFluent;
        c[ImGuiCol_FrameBg] = knobBackground;
        c[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.22f, 1.0f);
        c[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.22f, 0.28f, 1.0f);
        c[ImGuiCol_Button] = knobBackground;
        c[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.18f, 0.23f, 1.0f);
        c[ImGuiCol_ButtonActive] = accentNeon;
        c[ImGuiCol_SliderGrab] = accentNeon;
        c[ImGuiCol_SliderGrabActive] = accentNeon;
	}

    void Render(){
        frameCounter++;
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io->DisplaySize);

        if (ImGui::Begin("Window", nullptr, flags))
            Frame();
      
        ImGui::End();
        ImGui::Render();

        const float colors[4] = { 0.07f, 0.07f, 0.08f, 1.0f };
        DXManager::deviceContext->OMSetRenderTargets(1, &DXManager::rendererTargetView, nullptr);
        DXManager::deviceContext->ClearRenderTargetView(DXManager::rendererTargetView, colors);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        if (!DXManager::swapChain)
            return;

        DXManager::swapChain->Present(1, 0);
    };

    void DrawTextCenter(ImVec4 color, const char* text){
        float available = ImGui::GetContentRegionAvail().x;
        ImGui::SetCursorPosX((available - ImGui::CalcTextSize(text).x) * 0.5f);
        ImGui::TextColored(color, "%s", text);
    }

    void Frame() {
        ImGuiStyle& style = ImGui::GetStyle();

        const float size = 55.0f;
        float available = ImGui::GetContentRegionAvail().x;

        ImGui::BeginChild("Panel", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_None);
        ImGui::Spacing();
        ImGui::Spacing();

        DrawTextCenter(accentNeon, "Launch Control 3");

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        float centerX = (available - (size * 8 + size * 7)) * 0.5f;
        char label[32];
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + centerX);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(size, 16));

        for (int i = 0; i < 24; i++) {
            ImGui::BeginGroup();
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 center = ImVec2(pos.x + (size * 0.5f), pos.y + (size * 0.5f));

            sprintf(label, "%02d", i);
            if (ImGui::InvisibleButton(label, ImVec2(size, size))) {
                selected = i;
            }

            bool current = (selected == i);
            ImU32 color = ImGui::GetColorU32(current ? accentNeon : textBright);
            ImU32 borderColor = ImGui::GetColorU32(current ? accentNeon : (ImGui::IsItemHovered() ? textBright : borderFluent));
            ImU32 bgColor = ImGui::GetColorU32(MidiController::lastChanged == i ? accentSoft : knobBackground);

            if(i > 15){
                float half = size * 0.5f + 10;
                ImVec2 p_min = ImVec2(center.x - half, center.y - half / 2);
                ImVec2 p_max = ImVec2(center.x + half, center.y + half / 2);

                drawList->AddRectFilled(p_min, p_max, bgColor, 5.0f);
                drawList->AddRect(p_min, p_max, borderColor, 5.0f, 0, 1.5f);
                drawList->AddLine(ImVec2(center.x - 15, center.y), ImVec2(center.x + 15, center.y), color, 2.5f);
            }
            else{
                drawList->AddCircleFilled(center, size * 0.5f, bgColor, 32);
                drawList->AddCircle(center, size * 0.5f, borderColor, 32, 1.5f);
                drawList->AddLine(center, ImVec2(center.x, center.y - (size * 0.42f)), color, 2.5f);
            }

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (size * 0.5f) - (ImGui::CalcTextSize(label).x * 0.5f));
            ImGui::TextColored(current ? accentNeon : textDim, "%s", label);

            ImGui::EndGroup();
            if (i == 7 || i == 15) {
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + centerX);
            }
            else{
                ImGui::SameLine();
            }
        }

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();
        DrawTextCenter(accentNeon, "Properties");
        ImGui::Spacing();

        float btnWidth = 220.0f;
        if(selected <= 15){
            float total = (btnWidth * 2.0f) + 12.0f;
            float offset = (available - total) * 0.5f;

            ImGui::SetCursorPosX(offset);
            RecordShortcut(ConfigManager::config[selected].first, ImVec2(btnWidth, 40.0f));
            ImGui::SameLine(0.0f, 12.0f);
            RecordShortcut(ConfigManager::config[selected].second, ImVec2(btnWidth, 40.0f));
            ImGui::Spacing();
            DrawTextCenter(textDim, " Left | Right");
        }
        else{
            float offset = (available - btnWidth) * 0.5f;

            ImGui::SetCursorPosX(offset);
            RecordShortcut(ConfigManager::config[selected].first, ImVec2(btnWidth, 40.0f));
            ImGui::Spacing();
            DrawTextCenter(textDim, "Click");
        }
        ImGui::PopStyleVar();

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::EndChild();

        if (frameCounter % 100 == 0)
            MidiController::lastChanged = -1;
    }

    bool RecordShortcut(Shortcut& shortcut, ImVec2 size)
    {
        ImGui::PushID(&shortcut);

        std::string text = "";
        if (shortcut.ctrl)
            text += "Ctrl + ";

        if (shortcut.shift)
            text += "Shift + ";

        if (shortcut.alt)
            text += "Alt + ";

        if (shortcut.win)
            text += "Win + ";

        if (shortcut.imKey > 0) {
            text += ImGui::GetKeyName(static_cast<ImGuiKey>(shortcut.imKey));
        }
        else if(activeRecording != &shortcut) {
            text = "None";
        }
        else if(text.empty()){
            text = "...";
        }

        if (activeRecording != &shortcut)
        {
            if (ImGui::Button(text.c_str(), size)){
                shortcut.SetKey(-1);
                ConfigManager::Save();
                activeRecording = &shortcut;
            }
            ImGui::PopID();
            return false;
        }

        if (ImGui::Button(text.c_str(), size)){}

        ImGuiIO& io = ImGui::GetIO();
        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        {
            activeRecording = nullptr;
            ImGui::PopID();
            return false;
        }

        shortcut.ctrl = io.KeyCtrl;
        shortcut.shift = io.KeyShift;
        shortcut.alt = io.KeyAlt;
        shortcut.win = io.KeySuper;

        for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++)
        {
            ImGuiKey key = (ImGuiKey)i;
            if ((key == ImGuiKey_LeftCtrl || key == ImGuiKey_RightCtrl || key == ImGuiKey_LeftShift || key == ImGuiKey_RightShift || key == ImGuiKey_LeftAlt || key == ImGuiKey_RightAlt || key == ImGuiKey_LeftSuper || key == ImGuiKey_RightSuper))
                continue;

            if (key >= ImGuiKey_MouseLeft && key <= ImGuiKey_MouseWheelY)
                continue;

            if (key >= ImGuiKey_ReservedForModCtrl && key <= ImGuiKey_ReservedForModSuper)
                continue;

            if (ImGui::IsKeyPressed(key, false))
            {
                shortcut.SetKey(key);
                ConfigManager::Save();
                activeRecording = nullptr;
                ImGui::PopID();
                return true;
            }
        }
        ImGui::PopID();
        return false;
    }

    void Destroy(){
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
}