#pragma once

#include "DebugEngine.hpp"
#include "LibGdbConn.h"

struct DebugEngineGdb : public IDebugEngine {
    Iu_GdbConnection* conn = nullptr;
    
    DebugEngineGdb();
    ~DebugEngineGdb() override;
    
    void Start() override;
    void Shutdown() override;
};
