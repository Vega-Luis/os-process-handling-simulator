#ifndef PCB_H
#define PCB_H
#include <stdint.h>

typedef struct {
    uint32_t pid;
    uint32_t burst;
    uint8_t priority;
}ProgramControlBlock;

#endif