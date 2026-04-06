#ifndef SERVER_H
#define SERVER_H

int create_server_socket(int port);
int accept_client(int server_fd);

#endif // SERVER_H