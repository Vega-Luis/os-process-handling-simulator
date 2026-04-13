#include <stdio.h>
#include "metrics_display.h"
#include "job_metrics.h"

int turnaround_time(JobMetric *job) {
    return job->finish_time - job->arrival_time;
}

int waiting_time(JobMetric *job) {
    return turnaround_time(job) - job->burst;
}

void print_metrics() {
    printf("\nMétricas de los trabajos:\n");

    if (total_finished_jobs == 0) {
        printf("No se han terminado trabajos.\n");
        return;
    }

    printf("%-5s %-7s %-10s %-15s %-15s %-7s %-7s\n",
           "PID", "Burst", "Priority","Arrival Time", "Finish Time", "TAT", "WT");

    printf("---------------------------------------------------------------\n");

    int total_tat = 0;
    int total_wt = 0;
    int printed_jobs = 0;

    for (int i = 0; i < job_count; i++) {
        JobMetric *job = &job_metrics[i];

        if (!job->finished) continue;

        int tat = turnaround_time(job);
        int wt = waiting_time(job);

        total_tat += tat;
        total_wt += wt;
        printed_jobs++;

        printf("%-5u %-7u %-10u %-15d %-15d %-7d %-7d\n",
               job->pid,
               job->burst,
               job->priority,
               job->arrival_time,
               job->finish_time,
               tat,
               wt);
    }

    // Promedios basados en los realmente impresos
    float avg_tat = (float) total_tat / printed_jobs;
    float avg_wt  = (float) total_wt  / printed_jobs;

    printf("\nResumen:\n");
    printf("-------------------------------\n");
    printf("%-20s %d\n", "Trabajos terminados:", printed_jobs);
    printf("%-20s %.2f\n", "TAT promedio:", avg_tat);
    printf("%-20s %.2f\n", "WT promedio:", avg_wt);
    printf("-------------------------------\n");
}