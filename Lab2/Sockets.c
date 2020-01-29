/*
 This is for lecture 1/28/2020.
 
 Remember between layers we have an SAP (Service Access Point)
 
 Sockets api is the SAP between the application layer and the transport layer.
	 
*/

#include <sys/types.h>
#include <sys/socket.h>

struct sockaddr_in serveraddr, clientaddr;

int sersock, consock;
int len = sizeof(clientaddr);
char IP_ADDRESS = "129.186.23.166";

//step one:
// pf_inet means communication through internet.
// Sock stream means tcp socket. 0 means default protocol type which means tcp.
if ( (sersock = socket(PF_INET, SOCK_STREAM, 0)) < 0 ){
perror("socket() error:\n");
exit(1);
}

//Step two:
// Specify endpoint address for server socket.
serveraddr.sin_family = PF_INET;
//specify port number, htons helps specify byte order.
// htons() -> h = host to n= network s = short; 2bytes. htonl -> l = long; 4 bytes
serveraddr.sin_port = htons(80);
serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);


//Step 3: 
// bind assigns address and port to socket.
// after this step we have the first 4 parameters.
bind(sersock, (sockaddr *) &serveraddr, sizeof(serveraddr));

//Step 4:
// create passive mode socket
listen(sersock, 10);

//we now have to wait for the client to connect to fill in the remote ip and remote port parameter.

while(1){
	
	
	//the consock is created so we can connect to multiple clients at one time.
	consock = accept(sersock, (sockaddr *) &clientaddr, &len);

	//communication between server and client starts here	 
	read(consock, ..., ...);
	write(consock, ..., ...);
	 
	//communication between server and client ends here
	 close(consock);
}

close(sersock)



/////////////////////////////////////////////////////////////////////////////////
// client side

#include <sys/types.h>
#include <sys/socket.h>


struct sockaddr_in remoteaddr;

int clisock;
char SERVER_IP = "129.186.23.166";

if ( (clisock = socket(PF_INET, SOCK_STREAM, 0)) < 0 ){
perror("socket() error:\n");
exit(1);
}

remoteaddr.sin_family = PF_INET;
remoteaddr.sin_port = htons(80);
remoteaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

//Step 4:
//connect to the server. 
// 2nd & 3rd parameters are the addrees and port of the server.
connect(clisock, (sockaddr *) &remoteaddr, sizeof(remoteaddr));

// communication between client and server starts here

read(clisock, ..., ...);

write(clisock, ..., ...);

//communication between client and server ends here

close(clisock);

