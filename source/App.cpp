#include "App.hpp"

#include "imgui.h"
#include "ImGuiFileBrowser.h"
#include "ImGuiTextEditor.h"

void App::Show() {
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Load exe...")) {
            // TODO
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
    
    ImGui::Begin("Callstack");
    ImGui::End();
    
    ImGui::Begin("Watches");
    ImGui::End();
    
    ImGui::Begin("Breakpoints");
    ImGui::End();
    
    ImGui::Begin("Text File");
    ImGui::End();
}
