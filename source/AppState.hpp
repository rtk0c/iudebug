#pragma once

constexpr int kPathMax = 256;
constexpr int kRecentExeMaxCount = 16;

struct AppRuntimeState {
    bool persistStateModified = false;
};

// TODO(rtk0c): std::filesystem?
struct AppPersistentState {
    char gdbExePath[kPathMax];
    
    char recentExes[kPathMax][kRecentExeMaxCount];
    int recentExeCount;
    
    bool gdbPathInitializedFromUser;
    
    void Init();
    void Save();
};

extern AppRuntimeState* gAppRtState;
extern AppPersistentState* gAppPersistState;
