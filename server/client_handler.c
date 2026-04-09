#include "client_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "protocol.h"
#include "buffer.h"
#include "pcb.h"
#include "client_args.h"
#include "i_ready_queue.h"

void* manage_client(void* arg) {
    uint32_t pid_counter = 1;

    ClientArgs* client_args = (ClientArgs*)arg;
    int sock = client_args->client;
    IReadyQueue* ready_queue = client_args->ready_queue;

    while (1) {
        uint8_t buffer_data[REQUEST_SIZE];

        int bytes = recv(sock, buffer_data, REQUEST_SIZE, 0);
        if (bytes <= 0) {
            printf("Cliente desconectado\n");
            break;
        }
        Buffer buffer;
        buffer_init(&buffer, buffer_data, REQUEST_SIZE);
        Request req;
        deserialize_request(&buffer, &req);

        ProgramControlBlock pcb = {pid_counter, req.burst, req.priority};
        ready_queue->operations.enqueue(ready_queue, pcb);
        printf("[ENQUEUED] PID: %u, Burst: %u, Priority: %u\n", pcb.pid, pcb.burst, pcb.priority);

        uint8_t res_buffer[RESPONSE_SIZE];

        Buffer res_buf;
        buffer_init(&res_buf, res_buffer, RESPONSE_SIZE);
        Response res = {pid_counter++};
        serialize_response(&res_buf, &res);
        send(sock, res_buffer, res_buf.offset, 0);
    }
    close(sock);
    return NULL;
}
