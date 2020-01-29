#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/sysinfo.h>

struct sockaddr_in serveraddr, clientaddr;


int main(){
	int sersock, consock;
	int len = sizeof(clientaddr);
	char IP_ADDRESS[16] = "127.0.0.1";
	int server_port = 9388;
	char message[128] = "mission success";
	
	struct sysinfo s_info;

	sprintf(message,"%ld seconds sinceboot ", s_info.uptime);
	
	printf("\n%s\n", message);
	
	//step one:
	// pf_inet means communication through internet.
	// Sock stream means tcp socket. 0 means default protocol type which means tcp.
	if((sersock = socket(PF_INET, SOCK_STREAM, 0)) < 0 ){
		perror("socket() error:\n");
		return 0;
	}

	//Step two:
	// Specify endpoint address for server socket.
	serveraddr.sin_family = PF_INET;
	///specify port number, htons helps specify byte order.
	// htons() -> h = host to n= network s = short; 2bytes. htonl -> l = long; 4 bytes
	serveraddr.sin_port = htons(server_port);
	serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//Step 3: 
	// bind assigns address and port to socket.
	// after this step we have the first 4 parameters.
	int bind_status = bind(sersock, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
	
	if(bind_status != 0){
		printf("failed to bind to port\nexiting...\n");
		return 0;
	}
	


	//Step 4:
	// create passive mode socket
	int listen_status = listen(sersock, 10);
	
	if(listen_status != 0){
		printf("listening failed.\nexiting...\n");
		return 0;	
	}

	//we now have to wait for the client to connect to fill in the remote ip and remote port parameter.
	printf("Listening for the client.\n");
	
	while(1){	
		//the consock is created so we can connect to multiple clients at one time.
		consock = accept(sersock, (struct sockaddr *) &clientaddr, &len);

		//communication between server and client starts here	 
		
		write(consock,message,128);
								 
		//communication between server and client ends here
		close(consock);
		break;
	}
	printf("sent message to client\n");
	//close the socket
	close(sersock);
	return 0;
}

