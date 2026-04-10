#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "server.h"
#include "client_handler.h"
#include "job_scheduler_args.h"
#include "client_args.h"
#include "menu.h"
#include "i_ready_queue.h"
#include "time_manager.h"
#include "create_ready_queue.h"
#include "job_metrics.h"
#define PORT 8080

void* job_scheduler(void *arg) {
    JobSchedulerArgs* job_scheduler_args= (JobSchedulerArgs*)arg;
    int server_fd = job_scheduler_args->server_fd;
    IReadyQueue* ready_queue = job_scheduler_args->ready_queue;

    
    while (1) {
        // accept must lock until receives a client connection,
        // so it won't consume CPU while waiting
        int client = accept_client(server_fd);
        if (client < 0) {
            fprintf(stderr, "Error al aceptar cliente\n");
            continue;
        }
        printf("Cliente conectado: %d\n", client);
        ClientArgs* client_args = malloc(sizeof(ClientArgs));
        client_args->client = client;
        client_args->ready_queue = ready_queue;

        pthread_t hilo;
        pthread_create(&hilo, NULL, manage_client, client_args);
        pthread_detach(hilo);
    }
    return NULL;
}

void* cpu_scheduler(void* arg) {
    IReadyQueue* ready_queue = (IReadyQueue*)arg;

    while (1) {
            ProgramControlBlock pcb;
            int quantum;
            ready_queue->operations.dequeue(ready_queue, &pcb, &quantum);
            int time_slice = (quantum > 0 && quantum < (int) pcb.burst) ? quantum : pcb.burst;
            printf("[RUN]      PID: %u, Burst: %u, Priority: %u Time slice: %i\n", pcb.pid, pcb.burst, pcb.priority, time_slice);
            sleep(time_slice); // Simula la ejecución del proceso
            int new_burst = pcb.burst - (int)time_slice;
            if (new_burst > 0) {
                pcb.burst = (uint32_t)new_burst;
                ready_queue->operations.enqueue(ready_queue, pcb);
                printf("[REQUEUED] PID: %u, Burst: %u, Priority: %u\n", pcb.pid, pcb.burst, pcb.priority);
            } else {
                printf("[FINISHED] PID: %u\n", pcb.pid);
                int finish_time;
                pthread_mutex_lock(&time_mutex);
                finish_time = current_time;
                pthread_mutex_unlock(&time_mutex); 
                pthread_mutex_lock(&job_metrics_mutex);
                job_metrics[pcb.pid].finish_time = finish_time;
                job_metrics[pcb.pid].finished = 1;
                total_finished_jobs++;
                pthread_mutex_unlock(&job_metrics_mutex);
            }
    }
    return NULL;
}

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

    printf("%-5s %-7s %-15s %-15s %-7s %-7s\n",
           "PID", "Burst", "Arrival Time", "Finish Time", "TAT", "WT");

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

        printf("%-5u %-7u %-15d %-15d %-7d %-7d\n",
               job->pid,
               job->burst,
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


int main() {
    pthread_t timer_thread;
    pthread_create(&timer_thread, NULL, time_manager, NULL);

    int quantum = -1;
    SchedulerType scheduler_type = select_scheduler(&quantum);
    IReadyQueue* ready_queue = create_ready_queue(scheduler_type, quantum);

    int server_fd = create_server_socket(PORT);

    pthread_t job_scheduler_thread;

    JobSchedulerArgs* job_shceduler_args= malloc(sizeof(JobSchedulerArgs));
    job_shceduler_args->server_fd = server_fd;
    job_shceduler_args->ready_queue = ready_queue;


    pthread_create(&job_scheduler_thread, NULL, job_scheduler, job_shceduler_args);
    
    pthread_t cpu_scheduler_thread;
    pthread_create(&cpu_scheduler_thread, NULL, cpu_scheduler, ready_queue);

    printf("Presione Enter para mostrar las métricas de los trabajos...\n");
    getchar(); // consume posible '\n' previo
    getchar(); // espera Enter real
    print_metrics();
    pthread_join(cpu_scheduler_thread, NULL);
    pthread_join(job_scheduler_thread, NULL);

    return 0;
}