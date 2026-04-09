#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <pthread.h>
#include "i_ready_queue.h"
#include "pcb.h"


typedef struct {
    ProgramControlBlock* items;
    int size;
    int by_priority;
    int capacity;
    int quantum;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
} PriorityQueue;

PriorityQueue* pq_create(int capacity, int by_priority, int quantum);
int compare(PriorityQueue* pq, ProgramControlBlock a, ProgramControlBlock b );
void swap(ProgramControlBlock* a, ProgramControlBlock* b);
void heapify_up(PriorityQueue* pq, int index);
void heapify_down(PriorityQueue* pq, int index);
void pq_enqueue(IReadyQueue* rq, ProgramControlBlock pcb);
int pq_dequeue(IReadyQueue* rq, ProgramControlBlock* pcb, int* quantum);
void pq_destroy(IReadyQueue* rq);

#endif // PRIORITY_QUEUE_H