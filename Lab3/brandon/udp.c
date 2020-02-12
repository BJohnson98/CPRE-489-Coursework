#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>

struct sockaddr_in serveraddr;
struct sockaddr_in clientaddr;

int main(int argc, char **argv){
  int hsock, bind_status, recieve_status, send_status, random;
  char buf[1024];

  //seed random # gen
  srand(time(0));

  //take command line arguments and put them in variables:
  char listener_ip[16], destination_ip[16];
  strcpy(listener_ip, argv[1]);
  int listener_port = atoi(argv[2]);
  strcpy(destination_ip, argv[3]);
  int destination_port = atoi(argv[4]);
  int loss_rate = atoi( argv[5]);


  //step one: 
  //create socket for receiving data
  if(hsock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP) < 0){
    printf("Socket Error\n");
    return 0;
  }

  //step two:
  //specify endpoint addresses of vlc player we are listening to. 
  serveraddr.sin_family = PF_INET;
  serveraddr.sin_port = htons(listener_port);
  serveraddr.sin_addr.s_addr = inet_addr(listener_ip);

  //specify endpoint address of the client
  clientaddr.sin_family = PF_INET;
  clientaddr.sin_port = htons(destination_port);
  clientaddr.sin_addr.s_addr = inet_addr(destination_ip);

  //bind to port
  if((bind_status = bind(hsock, (struct sockaddr *) &serveraddr, sizeof(serveraddr))) < 0){
     printf("Failed to bind to port\n");
    return 0;
  }


  while(1){
    //generate a random number between 1 & 100.
    random = rand()%100;
  
    fgets(buf, 1024, stdin);
     // receive:
    if((recieve_status = recvfrom(hsock,(char *)buf, 1024, MSG_WAITALL, (struct sockaddr *) &serveraddr, (int *) sizeof(serveraddr))) < 0){
      printf("Receive Failed\n");
      //return 0;
    }

    //check if the random number is greater than the loss rate, if it is not, do not send packet.
    if(random > loss_rate){
      if((send_status = sendto(hsock, (char *)buf, 1024, MSG_CONFIRM, (const struct sockaddr *) &clientaddr, (int *) sizeof(serveraddr))) < 0){
        printf("Sending Failed\n");
        //return 0;
      }
    }


  }

  close();
  return 0;
}

