#pragma once

#include "BaseDefs.h"

IU_EXTERN_C_BEGIN

//~ Event object

typedef enum {
    Iu_EventType_PipeDataReady,
} Iu_EventType_;

typedef struct {
    void (*eventDeleter)(Iu_Event* object);
    Iu_EventType_ eventType;
} Iu_Event;

void Iu_FreeEvent(Iu_Event event);

//~ Event queue

// NOTE: opaque pointer
typedef struct Iu_EventQueue Iu_EventQueue;

Iu_EventQueue* Iu_CreateEventQueue();
void Iu_DestroyEventQueue(Iu_EventQueue* queue);

Iu_Event* Iu_PeekEvent(Iu_EventQueue* queue);
Iu_Event* Iu_DequeueEvent(Iu_EventQueue* queue);
void Iu_EnqueueEvent(Iu_EventQueue* queue, Iu_Event* event);

IU_EXTERN_C_END
