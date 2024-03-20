#pragma once

#include "BaseDefs.h"

#include <stddef.h>

IU_EXTERN_C_BEGIN

// NOTE: opaque pointer
typedef struct Iu_GdbConnection Iu_GdbConnection;

Iu_GdbConnection* Iu_GdbInit(const char* gdbExePath);

void Iu_GdbShutdown(Iu_GdbConnection* conn);

int Iu_GdbSendCommand(Iu_GdbConnection* conn, const char* fmt, ...);

// TODO head allocate or interned buffer?
typedef struct {
    char* msg;
    size_t msgSize;
} Iu_GdbMessage;

Iu_GdbMessage Iu_GdbGetOutput(Iu_GdbConnection* conn);
void Iu_GdbFreeOutput(Iu_GdbMessage* msg);

IU_EXTERN_C_END
