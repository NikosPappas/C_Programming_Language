#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>
#include <netinet/in.h>
#include <assert.h>
#define SIZE 4096

void send_file(FILE *,int);

int main(void){
    FILE *in=NULL;
    int socket_fd;
    int connect_ret;
    struct sockaddr_in server;

    socket_fd=socket(AF_INET,SOCK_STREAM,0);    
    if(socket_fd<0){
        printf("Error creating socket.\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("socket created sunccesfully.\n");
    }
    memset(&server,0,sizeof(server));
    server.sin_port=ntohs(12345);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.0");

    connect_ret=connect(socket_fd,(struct sockaddr *)&server,sizeof(server));
    if(connect_ret<0){
        printf("Error creating connect socket.\n");
        exit(EXIT_FAILURE);
    }
    in=fopen("file.txt","r");
    if(in==NULL){
        printf("Error opening file for reading\n");
    }
    else{
        printf("Starting to send file....\n");
    }
    send_file(in,socket_fd);
    return 0;

}
void send_file(FILE * in,int socket_fd){
    in=fopen("file.txt","r");
    int nread;
    unsigned char buffer[256];
    assert(in!=NULL);
    memset(&buffer,'\0',sizeof(buffer));
    fseek(in,0,SEEK_END);   
    long size=ftell(in);
    fseek(in,0,SEEK_SET);
    printf("File size %ld byte\n",size);
    while(1){
        nread=fread(buffer,sizeof(char),256,in);
        printf("%d bytes read\n",nread);
        printf("Total %ld bytes left to send\n",size-nread);
        size=size-nread;
        if(nread>0 || size!=0){
            write(socket_fd,buffer,nread);
        }
        else{
            break;
        }
    }
    printf("File uploaded succesfulyy.\n");
    fclose(in);
}
