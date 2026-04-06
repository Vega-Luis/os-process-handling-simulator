#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "server.h"
#include "client_handler.h"

#define PORT 8080


int main() {
    int server_fd = create_server_socket(PORT);


    // 5. Aceptar cliente
    while (1) {
        int client = accept_client(server_fd);
        printf("Cliente conectado\n");
        int* client_ptr = malloc(sizeof(int));
        *client_ptr = client;
        pthread_t hilo;
        pthread_create(&hilo, NULL, manage_client, client_ptr);
        pthread_detach(hilo);
    }

    return 0;
}