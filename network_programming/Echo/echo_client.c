#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc,char *argv[]){
	int socket_fd;
	int connect_ret;
	int recv_ret;
	char message[100];

	struct sockaddr_in server;
	memset(&server,0,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(12345);
	if(inet_aton("127.0.0.0",&server.sin_addr)==0){
		printf("error converting ip.\n");
		return -1;
	}
	socket_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(socket_fd<0){
		printf("socket error.\n");
		return -1;
	}
	connect_ret=connect(socket_fd,(struct sockaddr *)&server,sizeof(server));
	if(connect_ret<0){
		printf("connect error.\n");
		return -1;
	}
	printf("connect ok.\n");
	printf("Type a message: ");
	scanf("%s",message);
	while(strcmp(message,"exit")!=0){
		send(socket_fd,message,sizeof(message),0);
		recv_ret=recv(socket_fd,message,sizeof(message),0);
		printf("Message from server: %s\n",message);
		bzero(message,sizeof(message));
		scanf("%s",message);
	}
	close(socket_fd);

	return 0;
}
