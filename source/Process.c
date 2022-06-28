#include "Process.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#endif

static int Iu_PipeIsCreated(const Iu_Pipe* pipe) {
#ifdef _WIN32
    // TODO
#else
    return pipe->_fdRead != -1 || pipe->_fdWrite != -1;
#endif
    
}

void Iu_InitPipe(Iu_Pipe* pipe) {
#ifdef _WIN32
    // TODO
#else
    pipe->_fdRead = -1;
    pipe->_fdWrite = -1;
#endif
}

Iu_CPiS_ Iu_CreatePipe(Iu_Pipe* pipeIn) {
#ifdef _WIN32
    // TODO
#else
    int posixPipes[2];
    if (pipe(posixPipes) < 0) {
        return Iu_CPiS_GenericFailed;
    }
    
    for (int i = 0; i < IU_ARRAY_SIZE(posixPipes); i++) {
        int fd = posixPipes[i];
        int fstate = fcntl(fd, F_GETFL, 0);
        fstate |= O_NONBLOCK;
        fcntl(fd, F_SETFL, fstate);
    }
    
    pipeIn->_fdRead = posixPipes[0];
    pipeIn->_fdWrite = posixPipes[1];
#endif
    
    return Iu_CPiS_Success;
}

void Iu_DestroyPipe(Iu_Pipe* pipe) {
    if (!Iu_PipeIsCreated(pipe)) {
        return;
    }
    
#ifdef _WIN32
    // TODO
#else
    switch (Iu_PipeGetType(pipe)) {
        case Iu_PiT_Unbound: {
            close(pipe->_fdRead);
            close(pipe->_fdWrite);
        } break;
        
        case Iu_PiT_ReadEnd: {
            close(pipe->_fdRead);
        } break;
        
        case Iu_PiT_WriteEnd: {
            close(pipe->_fdWrite);
        } break;
    }
#endif
}

Iu_PiT_ Iu_PipeGetType(const Iu_Pipe* pipe) {
#ifdef _WIN32
    // TODO
#else
    if (pipe->_fdRead == -1) {
        return Iu_PiT_WriteEnd;
    } else if (pipe->_fdWrite == -1) {
        return Iu_PiT_ReadEnd;
    } else {
        return Iu_PiT_Unbound;
    }
#endif
}

#ifdef _WIN32
#else
static Iu_IoResult_ Iu_MapPosixErrnoToIoResult(int err) {
	switch (err) {
		case EFAULT: return Iu_IoResult_Eof;
		case EPERM: return Iu_IoResult_PermissionDenied;
		case ENOSPC: return Iu_IoResult_OutOfSpace;
		case EIO: return Iu_IoResult_Other;
        
		default: {
            fprintf(stderr, "Unimplemented POSIX errno %d, report bug immediately.", err);
            abort();
        }
	}
}
#endif

void Iu_PipeDataReadyEvent_Delete(Iu_Event* event) {
    // NOTE: Iu_Event is the first field, it has the same address as the actual allocation
    free(event);
}

Iu_IoResult_ Iu_PipeReadDataSync(Iu_Pipe* pipe, char* buffer, int size, int* transferred) {
#ifdef _WIN32
    // TODO
#else
    return Iu_IoResult_Success;
#endif
}

void Iu_PipeReadDataAsync(Iu_Pipe* pipe, int userId) {
    // TODO
}

Iu_IoResult_ Iu_PipeWriteData(Iu_Pipe* pipe, const char* buffer, int size, int* transferred) {
#ifdef _WIN32
    // TODO
#else
    int status = write(pipe->_fdWrite, buffer, size);
    if (status > 0) {
        if (transferred) {
            *transferred = status;
        }
        return Iu_IoResult_Success;
    } else {
        return Iu_MapPosixErrnoToIoResult(errno);
    }
#endif
}

static int Iu_ProcessIsCreated(const Iu_Process* process) {
#ifdef _WIN32
    // TODO
#else
    return process->_pid != 0;
#endif
    
}

void Iu_InitProcess(Iu_Process* process) {
#ifdef _WIN32
    // TODO
#else
    process->_pid = 0;
#endif
}

Iu_CPrS_ Iu_CreateProcess(Iu_Process* process, const char* exePath, const char** arguments, int argumentCount, Iu_Pipe* p2cPipe, Iu_Pipe* c2pPipe) {
#ifdef _WIN32
    // TODO
#else
    pid_t pid = fork();
    if (pid == -1) {
        return Iu_CPrS_GenericFailed;
    }
    
    if (pid == 0) {
        // Inside child process...
        
        if (p2cPipe) {
            // Close unused write end inside child process
            close(p2cPipe->_fdWrite);
            dup2(p2cPipe->_fdRead, STDIN_FILENO);
        }
        if (c2pPipe) {
            // Close unused read end inside child process
            close(c2pPipe->_fdRead);
            dup2(c2pPipe->_fdWrite, STDOUT_FILENO);
        }
        
        // An array of const char* with NULL padded at the end
        // NOTE: all memory from this process is reclaimed when exec() is called
        char** argumentsPosix = malloc(sizeof(char*) * (argumentCount + 1));
        for (int i = 0; i < argumentCount; i++) {
            // Copy string so we get a mutable one, required by execvp()
            int len = strlen(arguments[i]);
            char* arg = malloc(sizeof(char) * (len + 1));
            strcpy(arg, arguments[i]);
            
            argumentsPosix[i] = arg;
        }
        argumentsPosix[argumentCount] = NULL;
        
        execvp(exePath, argumentsPosix);
        
        _exit(0);
    } else {
        // Inside parent process...
        
        if (p2cPipe) {
            // Close unused read end inside parent process
            close(p2cPipe->_fdRead);
            p2cPipe->_fdRead = -1;
        }
        if (c2pPipe) {
            // Close unused write end inside parent process
            close(c2pPipe->_fdWrite);
            c2pPipe->_fdWrite = -1;
        }
        
        process->_pid = pid;
    }
#endif
    
    return Iu_CPrS_Success;
}

void Iu_TerminateProcess(Iu_Process* process) {
    if (!Iu_ProcessIsCreated(process)) {
        return;
    }
    
#ifdef _WIN32
    // TODO
#else
    kill(process->_pid, SIGTERM);
#endif
}
