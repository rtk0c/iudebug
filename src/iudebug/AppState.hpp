#pragma once

constexpr int kPathMax = 256;
constexpr int kRecentExeMaxCount = 16;

struct AppRuntimeState {
    bool persistStateModified = false;
};

enum ImGuiTheme {
    Dark,
    Light,
    Classic,
};

// TODO(rtk0c): std::filesystem?
struct AppConfig {
    ImGuiTheme theme;
    int fontSize;
    char fontFilePath[kPathMax];
    
    char dataFilePath[kPathMax];
    
    char gdbExePath[kPathMax];
    
    AppConfig();
};

struct AppPersistentState {
    void Init();
    void Save();
};

extern AppConfig* gAppConf;
extern AppRuntimeState* gAppRtState;
extern AppPersistentState* gAppPersistState;
