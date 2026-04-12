#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H
#include <pthread.h>

extern int current_time;
extern pthread_mutex_t time_mutex;

void* time_manager(void* arg);

#endif // TIME_MANAGER_H