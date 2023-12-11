#include "AppState.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFileBrowser.h"
#include "ImGuiTextEditor.h"
#include "DebugEngine.hpp"
#include "DebugEngineGdb.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/glad.h>
#include <cstdio>
#include <cstdlib>
#include <string>

static void GlfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

struct App {
    IDebugEngine* debugEngine;
    bool showDemoWindow = false;
    
    App() {
        // TODO(rtk0c): support for cppdbg on windows
        // TODO(hnsom): write some kind of custom in-process debug engine
        this->debugEngine = new DebugEngineGdb();
    }
    
    ~App() {
        delete debugEngine;
    }
    
    void ShowMenus() {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Load exe...")) {
                // TODO
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options")) {
            ImGui::MenuItem("ImGui demo window", nullptr, &showDemoWindow);
            ImGui::EndMenu();
        }
    }
    
    void Show() {
        auto& io = ImGui::GetIO();
        auto viewport = ImGui::GetMainViewport();
        
        //~ Setup main dockspace window, as well as the menu bar
        {
            constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            constexpr ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
            
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("MainViewport", nullptr, windowFlags);
            ImGui::PopStyleVar(3);
            
            ImGui::BeginMenuBar();
            this->ShowMenus();
            ImGui::EndMenuBar();
            
            auto dockspaceId = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
            
            ImGui::End();
        }
        
        auto& callstack = debugEngine->GetCallstack();
        
        //~ ImGui library windows
        if (showDemoWindow) {
            ImGui::ShowDemoWindow(&showDemoWindow);
        }
        
        //~ App windows
        ImGui::Begin("Callstack");
        if (ImGui::BeginTable("Callstack", 1)) {
            // TODO column headers
            
            for (auto& frame : callstack.frames) {
                // TODO
            }
            ImGui::EndTable();
        }
        ImGui::End();
        
        ImGui::Begin("Watches");
        ImGui::End();
        
        ImGui::Begin("Breakpoints");
        ImGui::End();
        
        ImGui::Begin("Text File");
        ImGui::End();
    }
};

int main() {
    glfwSetErrorCallback(&GlfwErrorCallback);
    if (!glfwInit()) {
        return -1;
    }    
    
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glslVersion = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glslVersion = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
    
    GLFWwindow* window = glfwCreateWindow(1280, 720, "IuDebug", nullptr, nullptr);
    if (window == nullptr) {
        return -2;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -3;
    }
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
    
    gAppConf = new AppConfig();
    gAppRtState = new AppRuntimeState();
    gAppPersistState = new AppPersistentState();
    gAppPersistState->Init();
    
    switch (gAppConf->theme) {
        using enum ImGuiTheme;
        case Dark: ImGui::StyleColorsDark(); break;
        case Light: ImGui::StyleColorsLight(); break;
        case Classic: ImGui::StyleColorsClassic(); break;
    }
    if (/*the string is not empty*/ gAppConf->fontFilePath[0] != '\0') {
        // TODO(hnsom): fontconfig? windows directwrite based lookup? macos whatever?
        io.Fonts->AddFontFromFileTTF(gAppConf->fontFilePath, gAppConf->fontSize);
    }
    
    ImVec4 clearColor(0.45f, 0.55f, 0.60f, 1.00f);
    App app;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        app.Show();
        if (gAppRtState->persistStateModified) {
            gAppRtState->persistStateModified = false;
            gAppPersistState->Save();
        }
        
        ImGui::Render();
        int dispalyWidth, displayHeight;
        glfwGetFramebufferSize(window, &dispalyWidth, &displayHeight);
        glViewport(0, 0, dispalyWidth, displayHeight);
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backupCurrentCtx = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupCurrentCtx);
        }
        
        glfwSwapBuffers(window);
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
