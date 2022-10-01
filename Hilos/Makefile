CC=gcc

%.o: %.c
	$(CC) -c -o $@ $<
all: socket_server socket_client

pthread01: socket_server.c
	gcc -pthread -o socket_server socket_server.o

pthread02: socket_client.o
	gcc -pthread -o socket_client socket_client.o
clean:
	rm -f *.o socket_server socket_client