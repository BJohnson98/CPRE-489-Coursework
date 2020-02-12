#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct sockaddr_in serveraddr, clientaddr;

int main(int argc, char** argv){
	int hsock;
	int random;
	char buf[4096];
	
	//Arguments that are passed to the program
	char listener_IP[20], destination_IP[20];
	strcpy(listener_IP, argv[1]);
	int listener_port = atoi(argv[2]);
	strcpy(destination_IP, argv[3]);
	int destination_port = atoi(argv[4]);
	int loss_rate = atoi(argv[5]);

	if((hsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket() error:\n");
		return 0;
	}
	
	//Specifies the endpoints
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(listener_port);
	serveraddr.sin_addr.s_addr = inet_addr(listener_IP);

	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(destination_port);
	clientaddr.sin_addr.s_addr = inet_addr(destination_IP);
	
	//Binds to port
	int rv = bind(hsock, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
	if(rv < 0){
		printf("failed to bind to port\nexiting...\n");
		return 0;
	}
	
	while(1){
		random = rand()%100;
		
		int len = sizeof(clientaddr);
		//Recieve message from listener port IP
		rv = recvfrom(hsock,(char *)buf, 4096, MSG_WAITALL,(struct sockaddr *)&serveraddr, &len);
		if(rv == -1){
			printf("Recive message failed");
			return 0;
		}
	
		//Send message to destination port and IP
		if(random > loss_rate){
			rv = sendto(hsock, (const char *)buf, 4096, MSG_CONFIRM, (const struct sockaddr *) &clientaddr, len);
			if(rv == -1){
				printf("Send message failed");
				return 0;
			}
		}
	}

	close();
	return 0;
}

