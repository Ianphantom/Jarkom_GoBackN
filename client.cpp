#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include <time.h>

#define PORT 4442


int main(int argc, char *argv[]){
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[32000];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");
	while(1){
		printf("Client: \t");
		char input[10000];
		char copyinput[10000];
		char perintah[100][10000];
		char *token;
		int i=0;  
		scanf(" %[^\n]s", input);
		strcpy(copyinput, input);
		token = strtok(input, " ");
		while( token != NULL ) {
			strcpy(perintah[i], token);
			i++;
			token = strtok(NULL, " ");
		}
		int wrongCommand = 0;
		// printf("ini input %s\n", input);
		if(strcmp(perintah[0], "WINDOW")==0){
            snprintf(buffer, sizeof buffer, "windowSize:%s", perintah[1]);
            send(clientSocket, buffer, strlen(buffer), 0);
		}else if(strcmp(perintah[0], "GET")==0){
            snprintf(buffer, sizeof buffer, "getData:%s", perintah[1]);
            send(clientSocket, buffer, strlen(buffer), 0);
        }else if(strcmp(perintah[0], ":exit")!=0){
			wrongCommand = 1;
			char peringatan[] = "Wrong Command";
			send(clientSocket, peringatan, strlen(peringatan), 0);
			// send(clientSocket, perintah[0], strlen(perintah[0]), 0);
		}

		if(strcmp(perintah[0], ":exit") == 0){
			send(clientSocket, perintah[0], strlen(perintah[0]), 0);
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}
		bzero(buffer, sizeof(buffer));
		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Server: \t%s\n", buffer);
		}
	}

	return 0;
}