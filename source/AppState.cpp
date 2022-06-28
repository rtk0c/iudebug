#include "AppState.hpp"

#include <cstdlib>
#include <cstdio>

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

static void GetAppConfigIniPath(char* path, int pathSize) {
    char cwdBuffer[kPathMax];
    GetCurrentDir(cwdBuffer, sizeof(cwdBuffer));
    snprintf(path, pathSize, "%s/iudebug.ini", cwdBuffer);
}

void AppPersistentState::Init() {
    char filePath[kPathMax];
    GetAppConfigIniPath(filePath, sizeof(filePath));
    
    FILE* file = fopen(filePath, "r");
    if (!file) return;
    
    
    
    fclose(file);
}

void AppPersistentState::Save() {
    char filePath[kPathMax];
    GetAppConfigIniPath(filePath, sizeof(filePath));
    
    FILE* file = fopen(filePath, "w");
    if (!file) return;
    
    fclose(file);
}

AppRuntimeState* gAppRtState = nullptr;
AppPersistentState* gAppPersistState = nullptr;
