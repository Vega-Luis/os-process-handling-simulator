#ifndef METRICS_DISPLAY_H
#define METRICS_DISPLAY_H
#include "job_metrics.h"

int turnaround_time(JobMetric *job);
int waiting_time(JobMetric *job);
void print_metrics();

#endif // DEFINE_METRICS_DISPLAY_H