#include <stdio.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>

struct sockaddr_in serveraddr, clientaddr;

int main(){
	struct tm* timeinfo;
	time_t rawtime;
	char buffer[80];
	int sersock, consock;
	int len = sizeof(clientaddr);
	char IP_ADDRESS[16] = "127.0.0.1";
	int server_port = 9388;
	char message[128];
	
	struct sysinfo s_info;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%I:%M%p", timeinfo);
	
   	int time_info = s_info.uptime;
	//convert time to seconds days, hours, minutes, and seconds
	int days = time_info / ( 24*3600);
	int hours = time_info % (24*3600)/3600;
	int minutes = (time_info % (24*3600*3600))/60;
	int seconds = (time_info % (24*3600*3600*60))/60;

	//put the system info into string called message
	sprintf(message,"%s:",IP_ADDRESS);
	sprintf(messafe,"%s %s up", message, buffer);
	if(days > 0){ sprintf(message,"%s %d days,",message, days); }
	if(hours > 0){ sprintf(message,"%s %d hours,",message, hours); }
	if(minutes > 0){ sprintf(message,"%s %d minutes,",message, minutes); }
	if(seconds > 0){ sprintf(message,"%s %d seconds,",message, seconds); }	
	// load averages
	sprintf(message,"%s, 1user, load average: %lu, %lu, %lu",message, loads[0], loads[1], loads[2]);

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
	//specify port number, htons helps specify byte order.
	// htons() -> h = host to n= network s = short; 2bytes. htonl -> l = long; 4 bytes
	serveraddr.sin_port = htons(server_port);
	serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//Step 3: 
	// bind assigns address and port to socket.
	// after this step we have the first 4 parameters.
	int bind_status = bind(sersock, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
	//exit if socket fails to bind.
	if(bind_status != 0){
		printf("failed to bind to port\nexiting...\n");
		return 0;
	}
	
	//Step 4:
	// create passive mode socket
	int listen_status = listen(sersock, 10);
	//exit if listening fails
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
