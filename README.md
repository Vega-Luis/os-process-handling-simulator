# os-process-handling-simulator
OS process scheduling simulator (FIFO, SJF, HPF, RR) with client-server communication using threads and sockets.

## Requirements
This project is meant to run on linux
A C compiler gcc and make are required

## Build & Run

### Server
From the repository root:

```bash
cd server
make
./server
```

Clean build artifacts:

```bash
make clean
```

### Client
In another terminal, from the repository root:

```bash
cd dummy-client
make
./client
```

Clean build artifacts:

```bash
make clean
```

## Note about the first connection
If you close the **server** first and then start it again, the client’s **first connection attempt may fail a couple of times**. If that happens, simply re-run the client (or retry the connection) and it should succeed.
