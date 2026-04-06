#ifndef READY_QUEUE_NODE_H
#define READY_QUEUE_NODE_H

#include "../pcb.h"

typedef struct ReadyQueueNode {
    ProgramControlBlock pcb;
    struct ReadyQueueNode* next;
} ReadyQueueNode;

#endif // READY_QUEUE_NODE_H