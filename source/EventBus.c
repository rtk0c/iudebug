#include "EventBus.h"

#include <stdlib.h>
#include <assert.h>

void Iu_FreeEvent(Iu_Event* event) {
    event->eventDeleter(event);
}

struct Iu_EventQueue {
    Iu_Event** ring;
    int ringLength;
    
    // Index of the head element
    int headIdx;
    // Index one past the tail element
    int tailIdx;
};

static int QueueGetElementCount(const Iu_EventQueue* queue) {
    if (queue->headIdx <= queue->tailIdx) {
        return queue->tailIdx - queue->headIdx;
    } else {
        return (queue->ringLength - queue->headIdx) + queue->tailIdx;
    }
}

static int QueueIsEmpty(const Iu_EventQueue* queue) {
    return QueueGetElementCount(queue) == 0;
}

Iu_EventQueue* Iu_CreateEventQueue() {
    Iu_EventQueue* queue = malloc(sizeof(Iu_EventQueue));
    
    queue->ringLength = 64;
    queue->ring = malloc(sizeof(Iu_Event*) * queue->ringLength);
    
    queue->headIdx = 0;
    queue->tailIdx = 0;
    
    return queue;
}

void Iu_DestroyEventQueue(Iu_EventQueue* queue) {
    free(queue->ring);
    free(queue);
}

Iu_Event* Iu_PeekEvent(Iu_EventQueue* queue) {
    if (QueueIsEmpty(queue)) {
        return NULL;
    }
    
    return queue->ring[queue->headIdx];
}

Iu_Event* Iu_DequeueEvent(Iu_EventQueue* queue) {
    if (QueueIsEmpty(queue)) {
        return NULL;
    }
    
    Iu_Event* result = queue->ring[queue->headIdx];
    queue->headIdx++;
    if (queue->headIdx >= queue->ringLength) {
        queue->headIdx = 0;
    }
    
    return result;
}

static void QueueResizeRing(Iu_EventQueue* queue, int newSize) {
    assert(newSize > queue->ringLength);
    
    Iu_Event** oldRing = queue->ring;
    Iu_Event** newRing = malloc(sizeof(Iu_Event*) * newSize);
    queue->ring = newRing;
    
    int i = queue->headIdx;
    int j = 0;
    while (i != queue->tailIdx) {
        newRing[j] = oldRing[i];
        
        i++;
        j++;
        if (i >= queue->ringLength) {
            i = 0;
        }
    }
    
    queue->headIdx = 0;
    queue->tailIdx = j;
    free(oldRing);
}

void Iu_EnqueueEvent(Iu_EventQueue* queue, Iu_Event* event) {
    if (// 1. Ring buffer filled: head is at 0, tail is at end
        (queue->headIdx == 0 && queue->tailIdx == queue->ringLength) ||
        // 2. Ring buffer filled: only one space left between tail and head
        // NOTE: we size when there is one space left to prevent head == tail, which can be confused with the state of elementCount == 0
        (queue->tailIdx + 1 == queue->headIdx))
    {
        int newSize = queue->ringLength * 2;
        QueueResizeRing(queue, newSize);
    }
    
    if (queue->tailIdx == queue->ringLength) {
        // Ring buffer is filled to the right, warp around to the beginning
        // Iu_EventQueue::headIdx > 0 must be true, since we checked that as condition (1) above
        queue->ring[0] = event;
        queue->tailIdx = 1;
    } else {
        queue->ring[queue->tailIdx] = event;
        queue->tailIdx++;
    }
}
