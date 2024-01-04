#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <assert.h>
#define SIZE 1024


void write_file(int);

int main(void){
    int socket_fd;
    int bind_ret;
    int inet_ret;
    int listen_ret;
    int accept_ret;
    socklen_t len;
    struct sockaddr_in server,client;
    socket_fd=socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd<0){
        printf("Error creating socket.\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("socket created succesfully.\n");
    }
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=ntohs(12345);
    inet_ret=inet_aton("127.0.0.0",&server.sin_addr);
    if(inet_ret<0){
        printf("Error converting ip.\n");
        exit(EXIT_FAILURE);
    }
    bind_ret=bind(socket_fd,(struct sockaddr *)&server,sizeof(server));
    if(bind_ret<0){
        printf("Error<binding ip error>\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("binding ok.\n");
    }
    listen_ret=listen(socket_fd,100);
    if(listen_ret<0){
        printf("Error listening for connections.\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("listening ok.\n");
    }
    len=sizeof(client);
    accept_ret=accept(socket_fd,(struct sockaddr *)&client,&len);
    if(accept_ret<0){
            printf("Error accepting new connection.\n");
    }
    else{
    printf("accepted new connection\n");
    write_file(accept_ret);
    }
    close(accept_ret);
    close(socket_fd);
    return 0;

}
void write_file(int socket_fd){
    char recvBuff[256];
    size_t bytesReceive;
    size_t totalBytes=0;
    FILE *f=fopen("file.txt","w");
    assert(f!=NULL);
    while((bytesReceive=read(socket_fd,recvBuff,256))>0){
        if(bytesReceive==-1){
            break;
        }
        printf("Bytes received: %ld\n",bytesReceive);
        fwrite(recvBuff,1,bytesReceive,f);
        totalBytes+=bytesReceive;
    }
    if(bytesReceive<0){
        printf("Read Error\n");
    }
    printf("Total bytes received %ld\n",totalBytes);
    fclose(f);

}
