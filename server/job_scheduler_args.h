#ifndef JOB_SCHEDULER_ARGS_H
#define JOB_SCHEDULER_ARGS_H

#include "ready_queue/ready_queue.h"

typedef struct {
    int server_fd;
    ReadyQueue* ready_queue;
} JobSchedulerArgs;

#endif // JOB_SCHEDULER_ARGS_H
