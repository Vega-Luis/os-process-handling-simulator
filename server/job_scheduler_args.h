#ifndef JOB_SCHEDULER_ARGS_H
#define JOB_SCHEDULER_ARGS_H

#include "i_ready_queue.h"

typedef struct {
    int server_fd;
    IReadyQueue* ready_queue;
} JobSchedulerArgs;

#endif // JOB_SCHEDULER_ARGS_H
