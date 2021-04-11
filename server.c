//503020200023
//clienr-server HW
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>

#define CLIENTS_NUMBER	20//clients number. Our server answer until 20 users

static unsigned int cli_count = 0;
static int user_id = 0;//our first user id


typedef struct {
	struct sockaddr_in addr;// Client remote address 
	int conn_file_descriptor;		
	int user_id;			
	char name[32];			// Client name 
} client_t;

client_t *clients[CLIENTS_NUMBER];


void queue_add(client_t *cl){
	int i;
	for(i=0;i<CLIENTS_NUMBER;i++){
		if(!clients[i]){
			clients[i] = cl;
			return;
		}
	}
}

/* Delete client from queue */
void queue_delete(int user_id){
	int i;
	for(i=0;i<CLIENTS_NUMBER;i++){
		if(clients[i]){
			if(clients[i]->user_id == user_id){
				clients[i] = NULL;
				return;
			}
		}
	}
}

/* Send message to all clients but the sender */
void send_message(char *s, int user_id){
	int i;
	for(i=0;i<CLIENTS_NUMBER;i++){
		if(clients[i]){
			if(clients[i]->user_id != user_id){
				if(write(clients[i]->conn_file_descriptor, s, strlen(s))<0){
					perror("write");
					exit(-1);
				}
			}
		}
	}
}

/* Send message to all clients */
void send_message_all(char *s){
	int i;
	for(i=0;i<CLIENTS_NUMBER;i++){
		if(clients[i]){
			if(write(clients[i]->conn_file_descriptor, s, strlen(s))<0){
				perror("write");
				exit(-1);
			}
		}
	}
}

/* Send message to sender */
void alert(const char *s, int conn_file_descriptor){
	if(write(conn_file_descriptor, s, strlen(s))<0){
		perror("write");
		exit(-1);
	}
}

/* Send message to client */
void direct_message(char *s, int user_id){
	int i;
	for(i=0;i<CLIENTS_NUMBER;i++){
		if(clients[i]){
			if(clients[i]->user_id == user_id){
				if(write(clients[i]->conn_file_descriptor, s, strlen(s))<0){
					perror("write");
					exit(-1);
				}
			}
		}
	}
}


/* Strip CRLF */
void strip_newline(char *s){
	while(*s != '\0'){
		if(*s == '\r' || *s == '\n'){
			*s = '\0';
		}
		s++;
	}
}


/* Handle all communication with the client */
void *handle_client(void *arg){
	char buff_out[2048];
	char buff_in[1024];
	int rlen;

	cli_count++;
	client_t *cli = (client_t *)arg;

	printf("---------------- New User Entered Room ----------------");
	printf("---------------- User ID is %d ----------------\n", cli->user_id);

	sprintf(buff_out, "---------------- Welcome %s ----------------\r\n", cli->name);
	send_message_all(buff_out);
	/* Receive input from client */
	while((rlen = read(cli->conn_file_descriptor, buff_in, sizeof(buff_in)-1)) > 0){
	        buff_in[rlen] = '\0';
	        buff_out[0] = '\0';
			strip_newline(buff_in);
			if(!strlen(buff_in)){
				continue;
			}

		/* Special options */
		if(buff_in[0] == '#'){
			char *command, *param;
			command = strtok(buff_in," ");
			if(!strcmp(command, "#exit")){
				sprintf(buff_out, "---------------- %s User Exitting Room ----------------\r\n", cli->name);
	send_message_all(buff_out);
	queue_delete(cli->user_id);
	printf("---------------- User ID %d Exitting ----------------\n", cli->user_id);
	free(cli);
	cli_count--;
	pthread_detach(pthread_self());
			}else if(!strcmp(command, "#newname")){
				param = strtok(NULL, " ");
				if(param){
					char *old_name = strdup(cli->name);
					strcpy(cli->name, param);
					sprintf(buff_out, "---------------- User Name, %s TO %s ----------------\r\n", old_name, cli->name);
					free(old_name);
					send_message_all(buff_out);
				}else{
					alert("---------------- Name can't be null ----------------\r\n", cli->conn_file_descriptor);
				}
			}else if(!strcmp(command, "#dm")){
				param = strtok(NULL, " ");
				if(param){
					int user_id = atoi(param);
					param = strtok(NULL, " ");
					if(param){
						sprintf(buff_out, "[DM][%s]", cli->name);
						while(param != NULL){
							strcat(buff_out, " ");
							strcat(buff_out, param);
							param = strtok(NULL, " ");
						}
						strcat(buff_out, "\r\n");
						direct_message(buff_out, user_id);
					}else{
						alert("---------------- Please Enter Message ----------------\r\n", cli->conn_file_descriptor);
					}
				}else{
					alert("---------------- Please Enter User ID ----------------\r\n", cli->conn_file_descriptor);
				}
			}else{
				alert("---------------- We Don't Have This Command ----------------\r\n", cli->conn_file_descriptor);
			}
		}else{
			snprintf(buff_out, sizeof(buff_out), "[%s] %s\r\n", cli->name, buff_in);
			send_message(buff_out, cli->user_id);
}
	}

	return NULL;
}

int main(int argc, char *argv[]){
	int listenfd = 0, conn_file_descriptor = 0;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	pthread_t tid;

	/* Socket settings */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	int option =1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

	/* Bind */
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
		perror("Socket binding failed");
		return 1;
	}

	/* Listen */
	if(listen(listenfd, 10) < 0){
		perror("Socket listening failed");
		return 1;
	}

	printf("<[SERVER STARTED]>\n");

	/* Accept clients */
	while(1){
		socklen_t clilen = sizeof(cli_addr);
		conn_file_descriptor = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

		/* Check if max clients is reached */
		if((cli_count+1) == CLIENTS_NUMBER){
			printf("<<MAX CLIENTS REACHED\n");
			printf("<<REJECT ");
			printf("\n");
			close(conn_file_descriptor);
			continue;
		}

		/* Client settings */
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->addr = cli_addr;
		cli->conn_file_descriptor = conn_file_descriptor;
		cli->user_id = user_id++;
		sprintf(cli->name, "%d", cli->user_id);

		/* Add client to the queue and fork thread */
		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void*)cli);

		/* Reduce CPU usage */
		sleep(1);
	}
}
