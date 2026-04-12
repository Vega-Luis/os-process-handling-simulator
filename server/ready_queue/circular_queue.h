#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stdlib.h>
#include <pthread.h>
#include "i_ready_queue.h"

#include "pcb.h"

typedef struct {
    ProgramControlBlock* items;
    int capacity;
    int size;
    int front;
    int rear;
    int quantum;

    int running;
    
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} CircularQueue;

CircularQueue* cq_create(int capacity, int quantum);
void cq_resize(CircularQueue* rq);
void cq_enqueue(IReadyQueue* rq, ProgramControlBlock pcb);
int cq_dequeue(IReadyQueue* rq, ProgramControlBlock* pcb, int* quantum);
void cq_destroy(IReadyQueue* rq);
void cq_shutdown(IReadyQueue* rq);

#endif //CIRCULAR_QUEUE_H