CC = gcc
CFLAGS = -Wall -g -I../shared -Iinclude -Ipid_manager -Itime_manager -Ijob_metrics


SRC=main.c ../shared/buffer.c ../shared/protocol.c ./client_handler.c ./server.c ./ready_queue/circular_queue.c ./ready_queue/priority_queue.c ./pid_manager/pid_manager.c ./time_manager/time_manager.c ./job_metrics/job_metrics.c system_control.c

all:
	$(CC) $(CFLAGS) $(SRC) -o server

clean:
	rm -f server 