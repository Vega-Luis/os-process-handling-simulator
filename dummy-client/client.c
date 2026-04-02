#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "protocol.h"
#include "buffer.h"

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1) {
        char option;
        

        printf("Enviar mensaje Y/N : ");
        scanf(" %c", &option);

        uint8_t buffer_data[REQUEST_SIZE];
        Buffer buffer;
        buffer_init(&buffer, buffer_data, REQUEST_SIZE);

        // This struct contains the data to be sent
        // The values are burst and priority
        // This data must be generated randomly or read from a file
        Request  req = {rand(), rand() % 256};

        serialize_request(&buffer, &req);

        send(sock, buffer_data, buffer.offset, 0);
        printf("Mensaje enviado: burst=%u, priority=%u\n", req.burst, req.priority);


        uint8_t res_buffer[RESPONSE_SIZE];
        int bytes = recv(sock, res_buffer, RESPONSE_SIZE, 0);
        if (bytes <= 0) {
            printf("Error al recibir respuesta del servidor\n");
            break;
        }
        Buffer res_buf;
        buffer_init(&res_buf, res_buffer, RESPONSE_SIZE);
        Response res;
        deserialize_response(&res_buf, &res);
        printf("Respuesta del servidor: pid=%u\n", res.pid);
    }

    return 0;
}