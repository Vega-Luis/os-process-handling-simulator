#ifndef REQUEST_MESSAGE_H
#define REQUEST_MESSAGE_H

#include <stdint.h>

typedef struct {
    uint32_t burst;
    uint8_t priority;
} Request;

#endif // REQUEST_MESSAGE_H