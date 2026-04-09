#ifndef I_READY_QUEUE_H
#define I_READY_QUEUE_H

#include "pcb.h"
#include "scheduler_type.h"

typedef struct IReadyQueue IReadyQueue;

typedef struct {
    void (*enqueue)(IReadyQueue* queue, ProgramControlBlock pcb);
    int (*dequeue)(IReadyQueue* queue, ProgramControlBlock* pcb, int* quantum);
} ReadyQueueOperations;

struct IReadyQueue {
    void* implementation;
    ReadyQueueOperations operations;
};

IReadyQueue* create_ready_queue(SchedulerType type, int quantum);
#endif // I_READY_QUEUE_H