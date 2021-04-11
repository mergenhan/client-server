//503020200023
//clienr-server HW
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
  
#define PORT 5000
#define BUF_SIZE 1024
#define BUF_SIZE 1024

  
int main(int argc, char**argv) {  

	//Variables are defined
	struct sockaddr_in addr, client_address;  
	int sockfd, flag;  
	char buffer[BUF_SIZE];  
	struct hostent * server;
	char * server_address;
	//if we dont have argument, this program will not work
	if (argc < 2) {
	printf("usage: client < ip address >\n");
	exit(1);  
	}
	//our server IP is parsed in here
	server_address = argv[1]; 
	//creating socket 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  
	//if we encounter some error, exit program
	if (sockfd < 0) {  
	printf("Error creating socket!\n");  
	exit(1);  
	}  
	printf("Socket created...\n");   
	//creating socket stuff
	memset(&addr, 0, sizeof(addr));  
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(server_address);
	addr.sin_port = htons(PORT);     

	flag = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
	//
	if (flag < 0) {  
	printf("Error connecting to the server!\n");  
	exit(1);  
	}  
	printf("Connected to the server...\n Now you can chat with your friend\n");  
	//reset buffer
	memset(buffer, 0, BUF_SIZE);
		int fdmax;
	fd_set main;
	fd_set read_fds;
	int i = 0;

// these variables are used for select function
// select function is used for dealing with multiple event sources
// We are dealing with chatroom so it is multiple event source.
	FD_ZERO(&main);
	    FD_ZERO(&read_fds);
	    FD_SET(0, &main);
	    FD_SET(sockfd, &main);
	fdmax = sockfd;
	while(1){
		read_fds = main;
		// handle return value of select functşon
		if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
			perror("select");
			exit(4);
		}
		
		for(i=0; i <= fdmax; i++ ){
			if(FD_ISSET(i, &read_fds)){
				//creating buffer for sending and recieving buffer
				char send_buffer[BUF_SIZE];
				char receive_buffer[BUF_SIZE];
				int number_of_received_bytes;
				
				if (i == 0){
					fgets(send_buffer, BUF_SIZE, stdin);
					if (!strcmp(send_buffer , "#exit\n") == 0) {
						send(sockfd, send_buffer, strlen(send_buffer), 0);
					}else{
						send(sockfd, send_buffer, strlen(send_buffer), 0);
						
						exit(0);

					}
				}else {
					number_of_received_bytes = recv(sockfd, receive_buffer, BUF_SIZE, 0);
					receive_buffer[number_of_received_bytes] = '\0';
					printf("%s\n" , receive_buffer);
					fflush(stdout);
				}
			}
		}
	}

	return 0;    
}  
