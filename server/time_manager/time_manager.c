#include <unistd.h>
#include "time_manager.h"
#include "../system_control.h"

int current_time = 0;
pthread_mutex_t time_mutex = PTHREAD_MUTEX_INITIALIZER;

void* time_manager(void* arg) {
    while (running) {
        sleep(1);
        pthread_mutex_lock(&time_mutex);
        current_time++;
        pthread_mutex_unlock(&time_mutex);
    }
    return NULL;
}