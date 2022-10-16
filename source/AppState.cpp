#include "AppState.hpp"

#include "IniReader.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace std::string_view_literals;

static void GetAppConfigIniPath(char* path, int pathSize) {
    char cwdBuffer[kPathMax];
    GetCurrentDir(cwdBuffer, sizeof(cwdBuffer));
    snprintf(path, pathSize, "%s/iudebug.ini", cwdBuffer);
}

static void IniGetPath(char* buffer, IniReader& reader, std::string_view section, std::string_view name) {
    auto opt = reader.GetString(section, name);
    if (opt)
        strncpy(buffer, opt->c_str(), kPathMax);
    else
        memset(buffer, 0, kPathMax);
}

void AppPersistentState::Init() {
    char filePath[kPathMax];
    GetAppConfigIniPath(filePath, sizeof(filePath));
    
    IniReader reader;
    if (reader.ParseFile(filePath) != 0) return;
    
    // Section: Appearance
    IniGetPath(fontFilePath, reader, "Appearance"sv, "Font"sv);
    fontSize = reader.GetInteger("Appearance"sv, "FontSize"sv).OrElse(16);
    {
        auto themeStr = reader.GetString("Appearance"sv, "Theme"sv);
        if (!themeStr || *themeStr == "dark")
            theme = ImGuiTheme::Dark;
        else if (*themeStr == "light")
            theme = ImGuiTheme::Light;
        else if (*themeStr == "classic")
            theme = ImGuiTheme::Classic;
        else
            // Default
            theme = ImGuiTheme::Dark;
    }
    
    // Section: DebugEngine.GDB
    IniGetPath(gdbExePath, reader, "DebugEngine.GDB"sv, "ExeFile"sv);
    
}

void AppPersistentState::Save() {
    char filePath[kPathMax];
    GetAppConfigIniPath(filePath, sizeof(filePath));
    
    FILE* file = fopen(filePath, "w");
    if (!file) return;
    
    // TODO(rtk0c): is there anything that we want to save?
    
    fclose(file);
}

AppRuntimeState* gAppRtState = nullptr;
AppPersistentState* gAppPersistState = nullptr;
