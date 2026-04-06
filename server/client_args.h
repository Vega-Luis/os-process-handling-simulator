#ifndef CLIENT_ARGS_H
#define CLIENT_ARGS_H

#include "ready_queue/ready_queue.h"

typedef struct {
    int client;
    ReadyQueue* ready_queue;
} ClientArgs;

#endif // CLIENT_ARGS_H