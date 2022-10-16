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
struct AppPersistentState {
    ImGuiTheme theme;
    int fontSize;
    char fontFilePath[kPathMax];
    
    char gdbExePath[kPathMax];
    
    void Init();
    void Save();
};

extern AppRuntimeState* gAppRtState;
extern AppPersistentState* gAppPersistState;
