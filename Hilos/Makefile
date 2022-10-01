CC=gcc

%.o: %.c
	$(CC) -c -o $@ $<
all: socket_server socket_client

socket_server: socket_server.o
	gcc -pthread -o socket_server socket_server.o

socket_client: socket_client.o
	gcc -pthread -o socket_client socket_client.o
clean:
	rm -f *.o socket_server socket_client