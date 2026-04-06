CC = gcc
CFLAGS = -Wall -g -I../shared


SRC=main.c ../shared/buffer.c ../shared/protocol.c ./client_handler.c ./server.c ./ready_queue/ready_queue.c

all:
	$(CC) $(CFLAGS) $(SRC) -o server

clean:
	rm -f server 