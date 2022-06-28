#pragma once

struct IDebugEngine {
    virtual ~IDebugEngine() = default;
    virtual void Start() = 0;
    virtual void Shutdown() = 0;
};
