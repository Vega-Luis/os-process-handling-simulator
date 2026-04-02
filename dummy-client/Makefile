CC = gcc
CFLAGS = -Wall -g -I../shared

SRC=client.c ../shared/buffer.c ../shared/protocol.c

all:
	$(CC) $(CFLAGS) $(SRC) -o client

clean:
	rm -f client 