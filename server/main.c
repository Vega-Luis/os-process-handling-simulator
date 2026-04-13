#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "server.h"
#include "menu.h"
#include "client_handler_args.h"
#include "i_ready_queue.h"
#include "time_manager.h"
#include "create_ready_queue.h"
#include "job_metrics.h"
#include "system_control.h"
#include "metrics_display.h"
#define PORT 8080

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

int main() {
    pthread_t timer_thread;
    pthread_create(&timer_thread, NULL, time_manager, NULL);

    int quantum = -1;
    SchedulerType scheduler_type = select_scheduler(&quantum);
    IReadyQueue* ready_queue = create_ready_queue(scheduler_type, quantum);

    int server_fd = create_server_socket(PORT);

    pthread_t client_handler_thread;

    ClientHandlerArgs* client_handler_args= malloc(sizeof(ClientHandlerArgs));
    client_handler_args->server_fd = server_fd;
    client_handler_args->ready_queue = ready_queue;


    pthread_create(&client_handler_thread, NULL, client_handler, client_handler_args);
    pthread_detach(client_handler_thread);
    
    pthread_t cpu_scheduler_thread;
    pthread_create(&cpu_scheduler_thread, NULL, cpu_scheduler, ready_queue);
    pthread_detach(cpu_scheduler_thread);

    int option;

    printf("Presione Enter para mostrar las métricas de los trabajos...\n");
    scanf("%d", &option); 

    printf("Deteniendo el servidor...\n");
    running = 0; // Detiene los hilos
    ready_queue->operations.shutdown(ready_queue);

    printf("Cerrando socket del servidor...\n");
    close(server_fd); // Cierra el socket para desbloquear accept



    printf("Esperando timer...\n");
    pthread_join(timer_thread, NULL);
    printf("Mostrando métricas...\n");
    print_metrics();

    return 0;
}