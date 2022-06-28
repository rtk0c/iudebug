#pragma once

#include "BaseDefs.h"
#include "EventBus.h"

#ifdef _WIN32
#else
#include <unistd.h>
#endif

IU_EXTERN_C_BEGIN

//~ IO events

typedef struct {
    Iu_Event baseEvent;
    char* data;
    int dataSize;
    int userId;
} Iu_PipeDataReadyEvent;
void Iu_PipeDataReadyEvent_Delete(Iu_Event* event);

//~ Pipe handling

// Any function taking a Iu_Pipe* object require the object be the initialized, except Iu_InitPipe() which initializes the object.
typedef struct {
#ifdef _WIN32
#else
    int _fdRead;
    int _fdWrite;
#endif
} Iu_Pipe;

void Iu_InitPipe(Iu_Pipe* pipe);

typedef enum {
    Iu_CPiS_Success,
    Iu_CPiS_GenericFailed,
} Iu_CPiS_;
Iu_CPiS_ Iu_CreatePipe(Iu_Pipe* pipe);

// NOTE: safe to call on uncreated pipes
void Iu_DestroyPipe(Iu_Pipe* pipe);

typedef enum {
    Iu_PiT_Unbound,
    Iu_PiT_ReadEnd,
    Iu_PiT_WriteEnd,
} Iu_PiT_;
Iu_PiT_ Iu_PipeGetType(const Iu_Pipe* pipe);

// Only works on Iu_PiT_ReadEnd pipes.
Iu_IoResult_ Iu_PipeReadDataSync(Iu_Pipe* pipe, char* buffer, int size, /*nullable*/ int* transferred);

// Only works on Iu_PiT_ReadEnd pipes.
void Iu_PipeReadDataAsync(Iu_Pipe* pipe, int userId);

// Only works on Iu_PiT_WriteEnd pipes.
Iu_IoResult_ Iu_PipeWriteData(Iu_Pipe* pipe, const char* buffer, int size, /*nullable*/ int* transferred);

//~ Process handling

// Any function taking a Iu_Process* object require the object be the initialized, except Iu_InitProcess() which initializes the object.
typedef struct {
#ifdef _WIN32
    HANDLE _handle;
#else
    pid_t _pid;
#endif
} Iu_Process;

void Iu_InitProcess(Iu_Process* process);

typedef enum {
    Iu_CPrS_Success,
    Iu_CPrS_GenericFailed,
} Iu_CPrS_;
Iu_CPrS_ Iu_CreateProcess(Iu_Process* process, const char* exePath, const char** arguments, int argumentCount, /*nullable*/ Iu_Pipe* p2cPipe, /*nullable*/ Iu_Pipe* c2pPipe);

// Request the process to terminate and wait for it to do so.
// NOTE: safe to call on uncreated processes
void Iu_TerminateProcess(Iu_Process* process);

IU_EXTERN_C_END
