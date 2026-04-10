#include "job_metrics.h"

JobMetric job_metrics[MAX_JOBS];
pthread_mutex_t job_metrics_mutex = PTHREAD_MUTEX_INITIALIZER;
int total_finished_jobs = 0;
int job_count = 0;