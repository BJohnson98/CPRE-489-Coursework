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
  int hsock, bind_status, recieve_status, send_status, randnum;
  char buf[1024];
  
  srand(time(0));

  //take command line arguments and put them in variables:
  char listener_ip[16], destination_ip[16];
  strcpy(listener_ip, argv[1]);
  int listener_port = atoi(argv[2]);
  strcpy(destination_ip, argv[3]);
  int destination_port = atoi(argv[4]);
  int loss_rate = atoi( argv[5]);


  //step one: 
  //create socket for recieving data
  if((hsock = socket(PF_INET, SOCK_DGRAM,0)) < 0){
    printf("Socket Error\n");
    return 0;
  }

  //step two:
  //specify endpoint addresses of vlc player we are listening to. 
  serveraddr.sin_family = PF_INET;
  serveraddr.sin_port = htons(listener_port);
  serveraddr.sin_addr.s_addr = htonl(listener_ip);

  //specify endpoint address of the client
  clientaddr.sin_family = PF_INET;
  clientaddr.sin_port = htons(destination_port);
 //inet addr??
 clientaddr.sin_addr.s_addr = htonl(destination_ip);
  
  //bind to port
  if((bind_status = bind(hsock, (struct sockaddr *) &serveraddr, sizeof(serveraddr))) < 0){
     printf("Failed to bind to port\n");
    return 0;
  }

  while(1){
  
    // recieve:
    if((recieve_status = recvfrom(hsock,(char *)buf, 1024, MSG_WAITALL, (struct sockaddr *) &serveraddr, sizeof(serveraddr))) < 0){
      printf("Recieve Failed\n");
     // break;
    }


   //generate a random number 
   randnum = rand() % 100;
   //only send the packet if the random number is greater than the loss rate.
   if(loss_rate < randnum){
     if((send_status = sendto(hsock, (char *)buf, 1024, MSG_CONFIRM, (const struct sockaddr *) &clientaddr, sizeof(clientaddr))) < 0){
       printf("Sending Failed\n");
     //  break;
     }
                                            }
   }
close();
  return 0;
}
