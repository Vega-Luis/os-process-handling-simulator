#include <pthread.h>
#include "pid_manager.h"

static uint32_t pid_counter = 1;
static pthread_mutex_t pid_mutex = PTHREAD_MUTEX_INITIALIZER;

uint32_t generate_pid() {
    pthread_mutex_lock(&pid_mutex);

    uint32_t pid = pid_counter++;

    pthread_mutex_unlock(&pid_mutex);
    return pid;
}

