#include <stdlib.h>
#include <stdio.h>

#include "ready_queue.h"

void* init_ready_queue() {
    ReadyQueue* rq = malloc(sizeof(ReadyQueue));
    rq->head = NULL;
    rq->tail = NULL;
    return rq;
}

void enqueue(IReadyQueue* queue, ProgramControlBlock pcb) {
    ReadyQueue* rq = (ReadyQueue*)queue->implementation;
    ReadyQueueNode* new_node = malloc(sizeof(ReadyQueueNode));

    new_node->pcb = pcb;
    new_node->next = NULL;

    if (rq->tail== NULL) {
        rq->head= new_node;
        rq->tail= new_node;
        return;
    }

    rq->tail->next = new_node;
    rq->tail= new_node;
}

int dequeue(IReadyQueue* queue, ProgramControlBlock* pcb) {
    ReadyQueue* rq = (ReadyQueue*)queue->implementation;
    if (is_empty(queue)) {
        return -1; // Queue is empty
    }

    ReadyQueueNode* temp = rq->head;
    *pcb = temp->pcb;
    rq->head = rq->head->next;

    if (rq->head == NULL) {
        rq->tail = NULL; // Queue is now empty
    }

    free(temp);
    return 0; // Success
}

int is_empty(IReadyQueue* queue) {
    ReadyQueue* rq = (ReadyQueue*)queue->implementation;
    return rq->head == NULL;
}

void print_queue(ReadyQueue* queue) {
    ReadyQueueNode* current = queue->head;
    while (current != NULL) {
        printf("PID: %u, Burst: %u, Priority: %u\n", current->pcb.pid, current->pcb.burst, current->pcb.priority);
        current = current->next;
    }
}

