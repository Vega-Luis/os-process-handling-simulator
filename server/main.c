#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "server.h"
#include "client_handler.h"
#include "ready_queue/ready_queue.h"
#include "job_scheduler_args.h"
#include "client_args.h"
#include <unistd.h>

#define PORT 8080



void* job_scheduler(void *arg) {
    JobSchedulerArgs* job_scheduler_args= (JobSchedulerArgs*)arg;
    int server_fd = job_scheduler_args->server_fd;
    ReadyQueue* ready_queue = job_scheduler_args->ready_queue;


    
    while (1) {
        int client = accept_client(server_fd);
        if (client < 0) {
            printf("Error al aceptar cliente\n");
            continue;
        }
        printf("Cliente conectado\n");
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
    ReadyQueue* ready_queue = (ReadyQueue*)arg;

    while (1) {
        if (!is_empty(ready_queue)) {
            ProgramControlBlock pcb;
            dequeue(ready_queue, &pcb);
            printf("Ejecutando proceso PID: %u, Burst: %u, Priority: %u\n", pcb.pid, pcb.burst, pcb.priority);
            sleep(pcb.burst); // Simula la ejecución del proceso
            printf("Proceso PID: %u finalizado\n", pcb.pid);
        } else {
            sleep(1); // Espera un segundo antes de revisar la cola nuevamente
        }
    }
    return NULL;
}


int main() {
    ReadyQueue ready_queue;
    init_ready_queue(&ready_queue);

    int server_fd = create_server_socket(PORT);

    pthread_t job_scheduler_thread;

    JobSchedulerArgs* job_shceduler_args= malloc(sizeof(JobSchedulerArgs));
    job_shceduler_args->server_fd = server_fd;
    job_shceduler_args->ready_queue = &ready_queue;


    pthread_create(&job_scheduler_thread, NULL, job_scheduler, job_shceduler_args);
    
    pthread_t cpu_scheduler_thread;
    pthread_create(&cpu_scheduler_thread, NULL, cpu_scheduler, &ready_queue);

    printf("main thread esperando a job_scheduler_thread\n");

    pthread_join(cpu_scheduler_thread, NULL);
    pthread_join(job_scheduler_thread, NULL);

    return 0;
}