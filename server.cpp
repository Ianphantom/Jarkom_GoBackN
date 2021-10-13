#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
 #include<iostream>
#include<ctime>
#include<cstdlib>

#define PORT 4442
using namespace std;

int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
                // printf("buffer %s\n", buffer);
				char *token;
				char buffercopy[32000];
				strcpy(buffercopy, buffer);
				char perintah[100][10000];
				token = strtok(buffercopy, ":");
				int i=0;
				int Number=0;
				while( token != NULL ) {
					strcpy(perintah[i], token);
					// printf("%s\n", perintah[i]);
					i++;
					token = strtok(NULL, ":");
				}
				if(strcmp(perintah[0], "windowSize")==0){
                    Number = atoi(perintah[1]);
                    // printf("%s", perintah[1]);
                    char peringatan[] = "WindowSize Saved";
                    // printf("windowsize : %d\n", windowSize);
                    send(newSocket, peringatan, strlen(peringatan), 0);
                    bzero(buffer, sizeof(buffer));
				}else if(strcmp(perintah[0], "getData")==0){
                    int nf = 10;
                    int i=1;
                    int no_tr = 0;
                    srand(time(NULL));
					int N = atoi(perintah[1]);
					while(i<=nf)
					{
						int x=0;
						for(int j=i;j<i+N && j<=nf;j++)
						{
							cout<<"Sent Frame "<<j<<endl;
							no_tr++;
							sleep(0.5);
						}
						for(int j=i;j<i+N && j<=nf;j++)
						{
							int flag = rand()%2;
							if(!flag)
								{
									cout<<"Acknowledgment for Frame "<<j<<endl;
									x++;
									sleep(0.5);
								}
							else
								{   cout<<"Frame "<<j<<" Not Received"<<endl;
									sleep(0.5);
									cout<<"Retransmitting Window"<<endl;
									sleep(1);
									break;
								}
						}
						cout<<endl;
						i+=x;
					}
                    char peringatan[] = "Success : Total number of transmission 10";
                    send(newSocket, peringatan, strlen(peringatan), 0);
                    bzero(buffer, sizeof(buffer));
                }
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Client: %s\n", buffer);
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);


	return 0;
}