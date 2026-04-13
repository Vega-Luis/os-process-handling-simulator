#ifndef CLIENT_HANDLER_ARGS_H
#define CLIENT_HANDLER_ARGS_H

#include "i_ready_queue.h"

typedef struct {
    int server_fd;
    IReadyQueue* ready_queue;
} ClientHandlerArgs;

#endif // CLIENT_HANDLER_ARGS_H
