#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char *argv[]){

	struct sockaddr_in server_sock;
	socklen_t server_len;
	char buffer[256];
	int connect_ret;
	int fd;
	ssize_t rec,sen;
	memset(&server_sock,'0',sizeof(server_sock));

	fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(fd<0){
		printf("Error creating socket.\n");
		exit(EXIT_FAILURE);
	}
	printf("socket ok.\n");
	server_sock.sin_family=AF_INET;
	server_sock.sin_port=htons(12345);
	server_sock.sin_addr.s_addr=inet_addr("127.0.0.1");
        server_len=sizeof(server_sock);
        connect_ret=connect(fd,(struct sockaddr *)&server_sock,server_len);
	if(connect_ret<0){
		printf("error conecting to server.\n");
		exit(EXIT_FAILURE);
	}
	bzero(buffer,sizeof(buffer));
        rec=recv(fd,buffer,sizeof(buffer),0);
	printf("Received from server: %s",buffer);
	close(fd);
	return 0;
}
