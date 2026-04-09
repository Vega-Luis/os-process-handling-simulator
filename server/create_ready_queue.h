#ifndef CREATE_READY_QUEUE_H
#define CREATE_READY_QUEUE_H

#include <stdlib.h>
#include "i_ready_queue.h"
#include "scheduler_type.h"
#include "./ready_queue/circular_queue.h"
#include "./ready_queue/priority_queue.h"

IReadyQueue* create_ready_queue(SchedulerType type) {
    IReadyQueue* ready_queue = malloc(sizeof(IReadyQueue));

    if (type == FIFO) {
        ready_queue->implementation = cq_create(512, -1);
        ready_queue->operations.enqueue = cq_enqueue;
        ready_queue->operations.dequeue = cq_dequeue;
    } else if (type == SJF) {
        ready_queue->implementation = pq_create(512, 0, -1);
        ready_queue->operations.enqueue = pq_enqueue;
        ready_queue->operations.dequeue = pq_dequeue;
    } else if (type == HPF) {
        ready_queue->implementation = pq_create(512, 1, -1);
        ready_queue->operations.enqueue = pq_enqueue;
        ready_queue->operations.dequeue = pq_dequeue;
    } else if (type == RR) {
        ready_queue->implementation = cq_create(512, 4);
        ready_queue->operations.enqueue = cq_enqueue;
        ready_queue->operations.dequeue = cq_dequeue;
    } else {
        ready_queue->implementation = cq_create(512, -1);
        ready_queue->operations.enqueue = cq_enqueue;
        ready_queue->operations.dequeue = cq_dequeue;
    }

    return ready_queue;
}

#endif // CREATE_READY_QUEUE_H