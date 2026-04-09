#include "circular_queue.h"

CircularQueue* cq_create(int capacity) {
    CircularQueue* rq = (CircularQueue*)malloc(sizeof(CircularQueue));
    size_t size = capacity * sizeof(ProgramControlBlock);
    rq->items = (ProgramControlBlock*)malloc(size);
    rq->capacity = capacity;
    rq->size = 0;
    rq->front = 0;
    rq->rear = 0;
    
    pthread_mutex_init(&rq->mutex, NULL);
    pthread_cond_init(&rq->cond, NULL);
    
    return rq;
}

void cq_resize(CircularQueue* rq) {
    int new_capacity = rq->capacity * 2;
    size_t size = new_capacity * sizeof(ProgramControlBlock);
    ProgramControlBlock* new_items = (ProgramControlBlock*)malloc(size);
    
    for (int i = 0; i < rq->size; i++) {
        new_items[i] = rq->items[(rq->front + i) % rq->capacity];
    }
    
    free(rq->items);
    rq->items = new_items;
    rq->front = 0;
    rq->rear = rq->size;
    rq->capacity = new_capacity;
}

void cq_enqueue(IReadyQueue* rq, ProgramControlBlock pcb) {
    CircularQueue* cq = (CircularQueue*)rq->implementation;

    pthread_mutex_lock(&cq->mutex);
    
    if ((cq->rear + 1) % cq->capacity == cq->front) {
        cq_resize(cq);
    }
    
    cq->items[cq->rear] = pcb;
    cq->rear = (cq->rear + 1) % cq->capacity;
    pthread_cond_signal(&cq->cond);
    pthread_mutex_unlock(&cq->mutex);
}

int cq_dequeue(IReadyQueue* rq, ProgramControlBlock* pcb) {
    CircularQueue* cq = (CircularQueue*)rq->implementation;
    pthread_mutex_lock(&cq->mutex);
    
    while (cq->front == cq->rear) {
        pthread_cond_wait(&cq->cond, &cq->mutex);
    }
    
    *pcb = cq->items[cq->front];
    cq->front = (cq->front + 1) % cq->capacity;
    
    pthread_mutex_unlock(&cq->mutex);
    return 0;
}

void cq_destroy(IReadyQueue* rq) {
    CircularQueue* cq = (CircularQueue*)rq->implementation;

    free(cq->items);
    pthread_mutex_destroy(&cq->mutex);
    pthread_cond_destroy(&cq->cond);
    free(cq);
}

int cq_is_empty(IReadyQueue* rq) {
    CircularQueue* cq = (CircularQueue*)rq->implementation;
    return cq->front == cq->rear;
}