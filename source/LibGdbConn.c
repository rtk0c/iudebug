#include "LibGdbConn.h"

#include "Process.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Iu_GdbConnection {
    Iu_Process process;
    Iu_Pipe p2cPipe;
    Iu_Pipe c2pPipe;
    char gdbOutBuffer[IU_MB(10)];
    char cmdBuffer[1024];
    char errBuffer[1024];
};

Iu_GdbConnection* Iu_GdbInit(const char* gdbExePath) {
    Iu_GdbConnection* conn = malloc(sizeof(Iu_GdbConnection));
    Iu_InitProcess(&conn->process);
    Iu_InitPipe(&conn->p2cPipe);
    Iu_InitPipe(&conn->c2pPipe);
    memset(conn->gdbOutBuffer, 0, sizeof(conn->gdbOutBuffer));
    memset(conn->cmdBuffer, 0, sizeof(conn->cmdBuffer));
    memset(conn->errBuffer, 0, sizeof(conn->errBuffer));
    
    if (Iu_CreatePipe(&conn->p2cPipe) != Iu_CPiS_Success ||
        Iu_CreatePipe(&conn->c2pPipe) != Iu_CPiS_Success)
    {
        Iu_GdbShutdown(conn);
        return NULL;
    }
    
    const char* gdbArgs[] = {
        "--interpreter=mi",
        "--quiet",
    };
    if (Iu_CreateProcess(&conn->process, gdbExePath, gdbArgs, IU_ARRAY_SIZE(gdbArgs), &conn->p2cPipe, &conn->c2pPipe) != Iu_CPrS_Success) {
        Iu_GdbShutdown(conn);
        return NULL;
    }
    
    return conn;
}

void Iu_GdbShutdown(Iu_GdbConnection* conn) {
    Iu_DestroyPipe(&conn->p2cPipe);
    Iu_DestroyPipe(&conn->c2pPipe);
    Iu_TerminateProcess(&conn->process);
    free(conn);
}

int Iu_GdbSendCommand(Iu_GdbConnection* conn, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(conn->cmdBuffer, sizeof(conn->cmdBuffer), fmt, args);
    va_end(args);
    
    if (written < 0) {
        // TODO error reporting requires POSIX functions, not cross platform
        //char errorMsg[256] = { 0 };
        //strerror_r(errno, errorMsg, sizeof(errorMsg));
        //snprintf(conn->errBuffer, sizeof(conn->errBuffer), "Failed to parse command: %s", errorMsg);
        return 0;
    } else if (written >= sizeof(conn->cmdBuffer)) {
        snprintf(conn->errBuffer, sizeof(conn->errBuffer), "Command exceeds buffer {%d > %d}", written, (int)sizeof(conn->cmdBuffer));
        return 0;
    }
    
    Iu_IoResult_ result = Iu_PipeWriteData(&conn->p2cPipe, conn->cmdBuffer, written, NULL);
    if (result != Iu_IoResult_Success) {
        // TODO if the command didn't get sent, maybe retry?
    }
    
    return 1;
}

Iu_GdbMessage Iu_GdbGetOutput(Iu_GdbConnection* conn) {
    int msgSize = 0;
    
    Iu_GdbMessage result;
    result.msg = conn->gdbOutBuffer;
    result.msgSize = msgSize;
    return result;
}

void Iu_GdbFreeOutput(Iu_GdbMessage* msg) {
    free(msg->msg);
    msg->msg = NULL;
    msg->msgSize = 0;
}
