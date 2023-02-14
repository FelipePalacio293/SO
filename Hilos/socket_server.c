#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<stdlib.h>
#include <pthread.h> 
#include <wait.h>

#define NUM_OF_THREADS 100

typedef struct __myarg_t {
	char name;
	char id;
} myarg_t;

void *worker(void *arg){
	
	myarg_t *m = (myarg_t *) arg;
	myarg_t *r = malloc(sizeof(myarg_t));
	
	pid_t pid = fork();
	if(pid == 0){
		if(strcmp(&m->id, "1") == 0){
			execlp("docker", "docker", "run", "-di", "--name", &m->name, "ubuntu:latest", "/bin/bash", "auto_assign_name: false", NULL);
		}
		else if(strcmp(&m->id, "2") == 0){
			execlp("docker", "docker", "ps", NULL);
		}
		else if(strcmp(&m->id, "3") == 0){
			execlp("docker", "docker", "stop", &m->name, NULL);
		}
		else if(strcmp(&m->id, "4") == 0){
			execlp("docker", "docker", "rm", &m->name, NULL);
		}
	}
	else{
		if(strcmp(&m->id, "1") == 0){
			strcpy(&r->name, "Created\n");
		}
		else if(strcmp(&m->id, "2") == 0){
			strcpy(&r->name, "Listed\n");
		}
		else if(strcmp(&m->id, "3") == 0){
			strcpy(&r->name, "Stopped\n");
		}
		else if(strcmp(&m->id, "4") == 0){
			strcpy(&r->name, "Deleted\n");
		}
	}
	return (void *) r;
}

int main(int argc , char *argv[]) {
	int socket_desc, client_sock, c, read_size;
	struct sockaddr_in server, client;  // https://github.com/torvalds/linux/blob/master/tools/include/uapi/linux/in.h
	char client_message[2000];
	
	// Create socket
    // AF_INET (IPv4 protocol) , AF_INET6 (IPv6 protocol) 
    // SOCK_STREAM: TCP(reliable, connection oriented)
    // SOCK_DGRAM: UDP(unreliable, connectionless)
    // Protocol value for Internet Protocol(IP), which is 0
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	pthread_t tid[NUM_OF_THREADS];
    /* An array with argument data for each thread. */
    myarg_t targ[NUM_OF_THREADS]; 
    /* Attributes (stack size, scheduling information) for the threads. */
    pthread_attr_t attr;
    /* Get default attributes for the threads. */
    pthread_attr_init(&attr);

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);
	
	//Bind the socket to the address and port number specified
	if( bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	// Listen
    // It puts the server socket in a passive mode, where it waits for the client 
    // to approach the server to make a connection. The backlog, defines the maximum 
    // length to which the queue of pending connections for sockfd may grow. If a connection 
    // request arrives when the queue is full, the client may receive an error with an 
    // indication of ECONNREFUSED.
	// https://man7.org/linux/man-pages/man2/listen.2.html
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	myarg_t *m;
	int contHilos = 0;
	while(1) {
		//accept connection from an incoming client
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if(client_sock < 0) {
			perror("accept failed");
			return 1;
		}
		puts("Connection accepted");
		while(1){			
			memset(client_message, 0, 2000);
			//Receive a message from client
			if(recv(client_sock, client_message, 2000, 0) > 0) {
				printf("received message: %s\n", client_message);
				fflush(stdin);
				//Send the message back to client
				if(strcmp(client_message, "1") == 0){
					// strcpy("Ingrese el nombre de la imagen\n", client_message);
					char server_message[] = "Ingrese el nombre de la imagen a crear";
					send(client_sock, server_message, strlen(server_message), 0);	
					memset(client_message, 0, 2000);
					if(recv(client_sock, client_message, 2000, 0) > 0) {
						targ[contHilos].name = *client_message;
						targ[contHilos].id = '1';
						pthread_create(&tid[contHilos], &attr, worker, &targ[contHilos]); 	
						pthread_join(tid[contHilos], (void **) &m);
						strcpy(server_message, &m->name);
						send(client_sock, server_message, strlen(server_message), 0);	
					}
				}
				else if(strcmp(client_message, "2") == 0){
					char server_message[] = "Lista";
					targ[contHilos].name = *client_message;
					targ[contHilos].id = '2';
					pthread_create(&tid[contHilos], &attr, worker, &targ[contHilos]); 	
					pthread_join(tid[contHilos], (void **) &m);
					strcpy(server_message, &m->name);	
					send(client_sock, server_message, strlen(server_message), 0);	
					// send(client_sock, server_message, strlen(server_message), 0);
				}
				else if(strcmp(client_message, "3") == 0){
					char server_message[] = "Ingrese el nombre de la imagen a detener";
					send(client_sock, server_message, strlen(server_message), 0);
					memset(client_message, 0, 2000);
					if(recv(client_sock, client_message, 2000, 0) > 0) {
						targ[contHilos].name = *client_message;
						targ[contHilos].id = '3';
						pthread_create(&tid[contHilos], &attr, worker, &targ[contHilos]);
						pthread_join(tid[contHilos], (void **) &m); 
						strcpy(server_message, &m->name);	
						send(client_sock, server_message, strlen(server_message), 0);	
					}
				}
				else if(strcmp(client_message, "4") == 0){
					char server_message[] = "Ingrese el nombre de la imagen a eliminar";
					send(client_sock, server_message, strlen(server_message), 0);
					memset(client_message, 0, 2000);
					if(recv(client_sock, client_message, 2000, 0) > 0) {
						printf("received message: %s\n", client_message);
						targ[contHilos].name = *client_message;
						targ[contHilos].id = '4';
						pthread_create(&tid[contHilos], &attr, worker, &targ[contHilos]); 	
						pthread_join(tid[contHilos], (void **) &m);
						strcpy(server_message, &m->name);
						send(client_sock, server_message, strlen(server_message), 0);	
					}
				}
				else{
					char server_message[] = "Error";
					send(client_sock, server_message, strlen(server_message), 0);	
				}
				contHilos++;
			}
		}
    }
	return 0;
}