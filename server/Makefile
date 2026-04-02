CC = gcc
CFLAGS = -Wall -g -I../shared


SRC=main.c ../shared/buffer.c ../shared/protocol.c

all:
	$(CC) $(CFLAGS) $(SRC) -o server

clean:
	rm -f server 