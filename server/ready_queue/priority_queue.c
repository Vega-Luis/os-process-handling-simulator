#include "priority_queue.h"
#include <stdlib.h>

PriorityQueue* pq_create(int capacity, int by_priority, int quantum) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->items = (ProgramControlBlock*)malloc(capacity * sizeof(ProgramControlBlock));
    pq->size = 0;
    pq->capacity = capacity;
    pq->by_priority = by_priority;
    pq->quantum = quantum;

    pthread_mutex_init(&pq->mutex, NULL);
    pthread_cond_init(&pq->cond, NULL);
    return pq;
}
int compare(PriorityQueue* pq, ProgramControlBlock a, ProgramControlBlock b) {
    if (pq->by_priority) {
        return (int)a.priority - (int)b.priority;
    } else {
        return (int)a.burst - (int)b.burst;
    }
}
void swap(ProgramControlBlock* a, ProgramControlBlock* b) {
    ProgramControlBlock temp = *a;
    *a = *b;
    *b = temp;
}
void heapify_up(PriorityQueue* pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (compare(pq, pq->items[index], pq->items[parent]) < 0) {
            swap(&pq->items[index], &pq->items[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void heapify_down(PriorityQueue* pq, int index) {
    while (1) {
        int l = 2*index+1, r = 2*index+2, s = index;

        if (l < pq->size && compare(pq, pq->items[l], pq->items[s]) < 0)
            s = l;
        if (r < pq->size && compare(pq, pq->items[r], pq->items[s]) < 0)
            s = r;

        if (s != index) {
            swap(&pq->items[index], &pq->items[s]);
            index = s;
        } else break;
    }
}

void pq_enqueue(IReadyQueue* rq, ProgramControlBlock pcb) {
    PriorityQueue* pq = (PriorityQueue*)rq->implementation;
    pthread_mutex_lock(&pq->mutex);
    if (pq->size == pq->capacity) return; // Queue is full
    pq->items[pq->size] = pcb;
    heapify_up(pq, pq->size);
    pq->size++;

    pthread_cond_signal(&pq->cond);
    pthread_mutex_unlock(&pq->mutex);
}

int pq_dequeue(IReadyQueue* rq, ProgramControlBlock* pcb, int* quantum) {
    PriorityQueue* pq = (PriorityQueue*)rq->implementation;
    pthread_mutex_lock(&pq->mutex);
    while (pq->size == 0) {
        pthread_cond_wait(&pq->cond, &pq->mutex);
    }
    *pcb = pq->items[0];
    pq->items[0] = pq->items[--pq->size];
    heapify_down(pq, 0);
    pthread_mutex_unlock(&pq->mutex);
    *quantum = pq->quantum;
    return 1;
}

void pq_destroy(IReadyQueue* rq) {
    PriorityQueue* pq = (PriorityQueue*)rq->implementation;
    pthread_mutex_destroy(&pq->mutex);
    pthread_cond_destroy(&pq->cond);
    free(pq->items);
    free(pq);
    free(rq);
}