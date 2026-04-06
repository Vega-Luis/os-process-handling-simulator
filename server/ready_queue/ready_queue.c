#include <stdlib.h>
#include <stdio.h>

#include "ready_queue.h"

void init_ready_queue(ReadyQueue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
}

void enqueue(ReadyQueue* queue, ProgramControlBlock pcb) {
    ReadyQueueNode* new_node = malloc(sizeof(ReadyQueueNode));

    new_node->pcb = pcb;
    new_node->next = NULL;

    if (queue->tail== NULL) {
        queue->head= new_node;
        queue->tail= new_node;
        return;
    }

    queue->tail->next = new_node;
    queue->tail= new_node;
}

int dequeue(ReadyQueue* queue, ProgramControlBlock* pcb) {
    if (is_empty(queue)) {
        return -1; // Queue is empty
    }

    ReadyQueueNode* temp = queue->head;
    *pcb = temp->pcb;
    queue->head = queue->head->next;

    if (queue->head == NULL) {
        queue->tail = NULL; // Queue is now empty
    }

    free(temp);
    return 0; // Success
}

int is_empty(ReadyQueue* queue) {
    return queue->head == NULL;
}

void print_queue(ReadyQueue* queue) {
    ReadyQueueNode* current = queue->head;
    while (current != NULL) {
        printf("PID: %u, Burst: %u, Priority: %u\n", current->pcb.pid, current->pcb.burst, current->pcb.priority);
        current = current->next;
    }
}

