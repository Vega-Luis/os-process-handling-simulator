#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include "ready_queue_node.h"

typedef struct {
    ReadyQueueNode* head;
    ReadyQueueNode* tail;
} ReadyQueue;

void init_ready_queue(ReadyQueue* queue);
void enqueue(ReadyQueue* queue, ProgramControlBlock pcb);
int dequeue(ReadyQueue* queue, ProgramControlBlock* pcb);
int is_empty(ReadyQueue* queue);
void print_queue(ReadyQueue* queue);

#endif // READY_QUEUE_H