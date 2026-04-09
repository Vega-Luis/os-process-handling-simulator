#ifndef CREATE_READY_QUEUE_H
#define CREATE_READY_QUEUE_H

#include <stdlib.h>
#include "i_ready_queue.h"
#include "scheduler_type.h"
#include "./ready_queue/circular_queue.h"

IReadyQueue* create_ready_queue(SchedulerType type) {
    IReadyQueue* ready_queue = malloc(sizeof(IReadyQueue));

    if (type == FIFO) {
        /*
        ready_queue->implementation = init_ready_queue();
        ready_queue->operations.enqueue = enqueue;
        ready_queue->operations.dequeue = dequeue;
        */
    } else if (type == RR) {
        ready_queue->implementation = cq_create(512);
        ready_queue->operations.enqueue = cq_enqueue;
        ready_queue->operations.dequeue = cq_dequeue;
    } else if (type == SJF || type == HPF) {
        /*
        ready_queue->implementation = heap_create(10, 0);
        ready_queue->implementation.enqueue = heap_enqueue;
        ready_queue->implementation.dequeue = heap_dequeue;
        ready_queue->implementation.destroy = heap_destroy;
        */
    } else {
        /*
        ready_queue->implementation = heap_create(10, 1);
        ready_queue->implementation.enqueue = heap_enqueue;
        ready_queue->implementation.dequeue = heap_dequeue;
        ready_queue->implementation.destroy = heap_destroy;
        */
    }

    return ready_queue;
}

#endif // CREATE_READY_QUEUE_H