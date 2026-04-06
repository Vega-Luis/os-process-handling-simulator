#include "client_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "protocol.h"
#include "buffer.h"

void* manage_client(void* arg) {
    int sock = *(int*)arg;

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
        printf("Recibido: burst=%u, priority=%u\n", req.burst, req.priority);

        uint8_t res_buffer[RESPONSE_SIZE];

        Buffer res_buf;
        buffer_init(&res_buf, res_buffer, RESPONSE_SIZE);
        Response res = {rand()}; // PID de ejemplo
        serialize_response(&res_buf, &res);
        send(sock, res_buffer, res_buf.offset, 0);
    }
    close(sock);
    return NULL;
}
