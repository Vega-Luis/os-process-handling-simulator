#ifndef CLIENT_ARGS_H
#define CLIENT_ARGS_H

#include "i_ready_queue.h"

typedef struct {
    int client;
    IReadyQueue* ready_queue;
} ClientArgs;

#endif // CLIENT_ARGS_H