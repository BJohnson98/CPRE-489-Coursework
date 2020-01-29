#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct sockaddr_in remoteaddr;

int main(){

        int clisock;
        char SERVER_IP[16] = "192.168.254.1";
        int server_port = 42069;
        char message[128];


        if((clisock = socket(PF_INET, SOCK_STREAM, 0)) < 0 ){
                perror("socket() error:\n");
                return 0;
        }

        remoteaddr.sin_family = PF_INET;
        remoteaddr.sin_port = htons(server_port);
        remoteaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

        //Step 4:
        ////connect to the server. 
        //// 2nd & 3rd parameters are the address and port of the server.
        int connect_status = connect(clisock, (struct sockaddr_in *) &remoteaddr, sizeof(remoteaddr));

        //if the connection status is not a success, then exit the program.
        if(connect_status != 0){
                return 0;
        }


        //// communication between client and server starts here
        read(clisock, message ,128);


        printf("\n%s\n", message);


        //communication between client and server ends here
        close(clisock);
}
