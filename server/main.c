#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "protocol.h"
#include "buffer.h"

#define PORT 8080

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


int main() {
    int server_fd;
    struct sockaddr_in address;

    // 1. Crear socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configurar dirección
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 3. Bind
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // 4. Escuchar
    listen(server_fd, 1);
    printf("Servidor escuchando en puerto %d...\n", PORT);

    // 5. Aceptar cliente
    while (1) {
        int client = accept(server_fd, NULL, NULL);
        pthread_t hilo;
        pthread_create(&hilo, NULL, manage_client, &client);
        pthread_detach(hilo);

    }
    close(server_fd);

    return 0;
}