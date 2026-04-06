#include "server.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

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