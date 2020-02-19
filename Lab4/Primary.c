#include <stdio.h> 
#include <string.h>    
#include <sys/socket.h>    
#include <stdlib.h>
#include "ccitt16.h"
#include "utilities.h"
#include "introduceerror.h"

void primary(int sockfd, double ber) {
	int readSize;
	int i;
	int win[3] = {0, 1, 2};
	char msg[DATA_LENGTH], srvReply[150];
	unsigned char packet[PACKET_SIZE];
	unsigned int packetNum = 0;
	struct packet_buffer buff[3];
	int k	
    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
		// fgets will load only DATA_LENGTH characters into msg at a time
		// however, if the user enters more than DATA_LENGTH characters,
		// the remaining characters will be loaded in DATA_LENGTH at a time
		// with each iteration of the loop
		//
		// ie if the user enters "HELLO<enter>", fgets will read in "HE", then
		// packetize it and send it. The program will come back to fgets and
		// read in "LL" and repeat until it sends "O"
		fgets(msg, DATA_LENGTH+1, stdin);
		// secondary is waiting for a *packet*, so we should packetize what
		// we're sending to it
		// TODO - make a send window of size three packets
		// Put the sent packets in a buffer until an ACK has been recieved
		// DATA_OFFSET = 2

		//Have a for loop to put three packets into the buffer
		for(i = 0; i < 3; i++){
			buildPacket(packet, DATA_PACKET, msg, packetNum++);
			//buff[i].packet = packet;
			//printfPacket(buff[i].packet[i]);
		}
   
		// 0 - Options and are not necessary here
		// If return value < 0, an error occured
	//For loop to send three packets
//	for(i = 0; i < 3; i++){
        	if( send(sockfd , buff[i].packet, PACKET_SIZE, 0) < 0)
            		perror("Send failed");
//	}
         
        // Receive a reply from the server
		// NOTE: If you have more data than 149 bytes, it will 
		// 			be received in the next call to "recv"
		// readSize - the length of the received message 
		// 				or an error code in case of failure
		// msg - a buffer where the received message will be stored
		// 149 - the size of the receiving buffer (any more data will be 
		// 		delievered in subsequent "recv" operations
		// 0 - Options and are not necessary here
	//For loop for receiving packets
//	for(i = 0; i < 3; i++){
        	if( (readSize = recv(sockfd , srvReply , 149 , 0)) < 0)
            		perror("recv failed");
//	}

	//TODO
	//svrReply is the packet we get back
	//srvReply[0] would have ACK or NAK
	//srvRelpy[1] would have the sequence number
	//If the sequence number is correct 
	//	Then we adjust the send window and removes packet from the buffer 
	//	If something is wrong we will have to resend packets
	//		To do this we will need to adjust send window and
	//		retransmit all packets in send window
	

		// In general, secondary responds with only ACK or NAK *packets*
		// so we should print them with printPacket(x)
        printf("Server's reply:\n");
		printPacket(srvReply);
    }
  
}
