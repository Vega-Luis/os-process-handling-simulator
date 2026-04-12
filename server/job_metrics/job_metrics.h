#ifndef JOB_METRICS_TABLE_H
#define JOB_METRICS_TABLE_H

#include <stdint.h>
#include <pthread.h>

#define MAX_JOBS 1000

typedef struct {
    uint32_t pid;
    uint32_t burst;
    uint32_t priority;

    int arrival_time;
    int finish_time;

    int finished;
} JobMetric;

extern JobMetric job_metrics[MAX_JOBS];
extern pthread_mutex_t job_metrics_mutex;
extern int total_finished_jobs;
extern int job_count;

#endif // JOB_METRICS_TABLE_H