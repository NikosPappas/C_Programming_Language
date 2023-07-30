#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
void handle_client_connection(int);
int main(int argc,char *argv[]){
        pid_t pid;
	int socket_fd;
	int bind_ret;
	int listen_ret;
	int accept_fd;
        int send_ret;
	int recv_ret;
	char message[100];
	struct sockaddr_in server,client;
	socklen_t client_len;
        memset(&server,0,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=ntohs(12345);
	if(inet_aton("127.0.0.0",&server.sin_addr)==0){
		printf("error converting ip address.\n");
		return -1;
	}
	socket_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(socket_fd<0){
		printf("socket error.\n");
		return -1;
	}
	printf("socket ok.\n");
	bind_ret=bind(socket_fd,(struct sockaddr *)&server,sizeof(server));
	if(bind_ret<0){
		printf("bind error.\n");
		return -1;
	}
	printf("bind ok.\n");
	listen_ret=listen(socket_fd,20);
	if(listen_ret<0){
		printf("listen error.\n");
		return -1;
	}
	printf("listen ok.\n");
	while(1){
		client_len=sizeof(socklen_t);
		accept_fd=accept(socket_fd,(struct sockaddr *)&client,&client_len);
		if(accept_fd<0){
			printf("accept error.\n");
		}
		printf("Client: %s port:%d\n",inet_ntoa(client.sin_addr),client.sin_port);
    pid=fork();
		if(pid<0){
			printf("Error creating new socket.\n");
			return -1;
		}
		else if(pid==0){
			close(socket_fd);
			handle_client_connection(accept_fd);
			close(accept_fd);
			exit(0);
		}
		else{
			//inside parent procedure
			//wait until the child finishes.
			wait(NULL);
			//close accept connection
			close(accept_fd);
		}
	}

	return 0;
}
void handle_client_connection(int fd){
	char message[100];
	int recv_ret;
	int send_ret;
	recv_ret=recv(fd,message,sizeof(message),0);
	if(recv_ret!=sizeof(message)){
		printf("receive error.\n");
		return;
	}
	printf("Received from client: <%s>\n",message);
	printf("Sending back message: <%s>\n",message);
	send_ret=send(fd,message,sizeof(message),0);
	if(send_ret!=sizeof(message)){
		printf("send error.\n");
		return ;
	}
	printf("send ok.\n");
}
