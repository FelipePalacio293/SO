#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

void recibirMensajeServidor(int sock){
	char server_reply[2000];
	memset(server_reply, 0, 2000);
	if (recv(sock, server_reply, 2000, 0) < 0) {
		puts("recv failed");
	} else {
		puts("recv ok");
	}
	puts("Server reply :");
	puts(server_reply);
}

int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_in server;
	char message[1000], server_reply[2000];
	
	//Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");

	while(1)
	{
		printf("Opciones:\n1. Crear Contenedor\n2. Listar Contenedores\n3. Detener Contenedores\n4. Borrar contenedor\n");
		
		fgets(message, 1000, stdin);
		message[strcspn(message, "\n")] = 0;
		if (send(sock, message, strlen(message)+1, 0) < 0) {
			puts("Send failed");
			return 1;
		} else {
			puts("send ok");
		}
		

	
		
		
		if(strcmp(message, "1") == 0){
			recibirMensajeServidor(sock);
			fgets(message, 1000, stdin);
			message[strcspn(message, "\n")] = 0;
			if (send(sock, message, strlen(message)+1, 0) < 0) {
				puts("Send failed");
				return 1;
			} else {
				puts("send ok");
			}
			recibirMensajeServidor(sock);
		}
		else if(strcmp(message, "2") == 0){
			// recibirMensajeServidor(sock);
			recibirMensajeServidor(sock);

		}
		else if(strcmp(message, "3") == 0){
			recibirMensajeServidor(sock);
			fgets(message, 1000, stdin);
			message[strcspn(message, "\n")] = 0;
			if (send(sock, message, strlen(message)+1, 0) < 0) {
				puts("Send failed");
				return 1;
			} else {
				puts("send ok");
			}
			recibirMensajeServidor(sock);

		}
		else if(strcmp(message, "4") == 0){
			recibirMensajeServidor(sock);
			fgets(message, 1000, stdin);
			message[strcspn(message, "\n")] = 0;
			if (send(sock, message, strlen(message)+1, 0) < 0) {
				puts("Send failed");
				return 1;
			} else {
				puts("send ok");
			}
			recibirMensajeServidor(sock);
		}	
		
	}
	close(sock);
	return 0;
}