#pragma once

#include <vector>
#include <string>

struct CallstackFrame {
    std::string stackframeName;
    // TODO
};

struct Callstack {
    std::vector<CallstackFrame> frames;
};

struct IDebugEngine {
    virtual ~IDebugEngine() = default;
    virtual void Start() = 0;
    virtual void Shutdown() = 0;
    virtual Callstack& GetCallstack() = 0;
};
