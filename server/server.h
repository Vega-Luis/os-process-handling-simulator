#ifndef SERVER_H
#define SERVER_H

int create_server_socket(int port);
int accept_client(int server_fd);
void* client_handler(void* arg);

#endif // SERVER_H