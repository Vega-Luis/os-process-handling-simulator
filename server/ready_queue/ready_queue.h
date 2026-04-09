#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include "ready_queue_node.h"
#include "../i_ready_queue.h"

typedef struct {
    ReadyQueueNode* head;
    ReadyQueueNode* tail;
} ReadyQueue;

void* init_ready_queue();
void enqueue(IReadyQueue* queue, ProgramControlBlock pcb);
int dequeue(IReadyQueue* queue, ProgramControlBlock* pcb);
int is_empty(IReadyQueue* queue);
void print_queue(ReadyQueue* queue);

#endif // READY_QUEUE_H