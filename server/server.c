#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


#include "client_args.h"
#include "client_handler_args.h"
#include "client_handler.h"
#include "system_control.h"



int create_server_socket(int port) {
    int server_fd;
    struct sockaddr_in client_address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = INADDR_ANY;
    client_address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&client_address, sizeof(client_address));

    listen(server_fd, 5);
    printf("Servidor escuchando en puerto %d...\n", port);
    return server_fd;
}

int accept_client(int server_fd) {
    return accept(server_fd, NULL, NULL);
}

void* client_handler(void *arg) {
    ClientHandlerArgs* client_handler_args= (ClientHandlerArgs*)arg;
    int server_fd = client_handler_args->server_fd;
    IReadyQueue* ready_queue = client_handler_args->ready_queue;

    
    while (running) {
        // accept must lock until receives a client connection,
        // so it won't consume CPU while waiting
        int client = accept_client(server_fd);
        if (client < 0) {
            if(!running) break;
            fprintf(stderr, "Error al aceptar cliente\n");
            continue;;
        }
        printf("Cliente conectado: %d\n", client);
        ClientArgs* client_args = malloc(sizeof(ClientArgs));
        client_args->client = client;
        client_args->ready_queue = ready_queue;

        pthread_t hilo;
        pthread_create(&hilo, NULL, manage_client, client_args);
        pthread_detach(hilo); 
    }
    printf("Jos sale");
    return NULL;
}