#include "schedulers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "protocol.h"
#include "buffer.h"
#include "pcb.h"
#include "client_args.h"
#include "i_ready_queue.h"
#include "pid_manager.h"
#include "time_manager.h"
#include "job_metrics.h"
#include "system_control.h"

void* job_scheduler(void* arg) {

    ClientArgs* client_args = (ClientArgs*)arg;
    int client_fd = client_args->client;
    IReadyQueue* ready_queue = client_args->ready_queue;

    while (running) {
        uint8_t buffer_data[REQUEST_SIZE];
        printf("Esperando datos del cliente %d...\n", running);


        int bytes = recv(client_fd, buffer_data, REQUEST_SIZE, 0);
        printf("Recibidos %d bytes del cliente %d\n", bytes, running);
        if (bytes <= 0) {
            printf("Cliente desconectado\n");
            break;
        }
        Buffer buffer;
        buffer_init(&buffer, buffer_data, REQUEST_SIZE);
        Request req;
        deserialize_request(&buffer, &req);

        ProgramControlBlock pcb = {generate_pid(), req.burst, req.priority};
        ready_queue->operations.enqueue(ready_queue, pcb);
        printf("[ENQUEUED] PID: %u, Burst: %u, Priority: %u\n", pcb.pid, pcb.burst, pcb.priority);
        int arrival_time;
        pthread_mutex_lock(&time_mutex);
        arrival_time = current_time;
        pthread_mutex_unlock(&time_mutex);
        pthread_mutex_lock(&job_metrics_mutex);
        job_metrics[pcb.pid].pid = pcb.pid;
        job_metrics[pcb.pid].burst = pcb.burst;
        job_metrics[pcb.pid].priority = pcb.priority;
        job_metrics[pcb.pid].arrival_time = arrival_time;
        job_count++;
        pthread_mutex_unlock(&job_metrics_mutex);

        uint8_t res_buffer[RESPONSE_SIZE];

        Buffer res_buf;
        buffer_init(&res_buf, res_buffer, RESPONSE_SIZE);
        Response res = {pcb.pid};
        serialize_response(&res_buf, &res);
        send(client_fd, res_buffer, res_buf.offset, 0);
    }
    printf("Cerrando conexión con el cliente\n");
    close(client_fd);
    free(client_args);
    return NULL;
}

void* cpu_scheduler(void* arg) {
    IReadyQueue* ready_queue = (IReadyQueue*)arg;

    while (running) {
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
