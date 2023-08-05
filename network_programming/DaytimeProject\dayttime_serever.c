#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void handle_request(int);

int main(int argc,char *argv[]){

	pid_t pid;
	int socket_fd;
	int bind_ret;
	int listen_ret;
	int accept_fd;
        char buffer[100];
	struct sockaddr_in server,client;
	socklen_t client_len;

	memset(&server,'0',sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=ntohs(12345);
	if(inet_aton("127.0.0.1",&server.sin_addr)<0){
		printf("error converting ip.\n");
		exit(EXIT_FAILURE);
	}

	socket_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(socket_fd<0){
		perror("socket_creation");
		exit(EXIT_FAILURE);
	}
	printf("socket ok.\n");
	bind_ret=bind(socket_fd,(struct sockaddr *)&server,sizeof(server));
	if(bind_ret<0){
		perror("bind error");
		exit(EXIT_FAILURE);
	}
	printf("bind ok.\n");

	listen_ret=listen(socket_fd,20);
	if(listen_ret<0){
		perror("listen error");
		exit(EXIT_FAILURE);
	}
	printf("listen ok\n");
	while(1){
		client_len=sizeof(client);
		accept_fd=accept(socket_fd,(struct sockaddr *)&client,&client_len);
		if(accept_fd<0){
			perror("accept_fd error");
		        exit(EXIT_FAILURE);
		}
	        printf("accept ok.\n");
		printf("client :%s at port: %d\n",inet_ntoa(client.sin_addr),client.sin_port);
		pid=fork();
		if(fork<0){
			perror("fork");
		}
		else if(pid==0){
			close(socket_fd);
			handle_request(accept_fd);
			close(accept_fd);
			
		}
		else{
			wait(NULL);
			close(accept_fd);
		}
	}

	return 0;
}
void handle_request(int fd){
	time_t now;
	time(&now);

	char buffer[100];
	bzero(buffer,sizeof(buffer));
	strcpy(buffer,ctime(&now));
	ssize_t size=send(fd,buffer,sizeof(buffer),0);
	if(size<0){
		printf("send error.\n");
		return ;
	}
	printf("Message sent to server: %s",buffer);
}
