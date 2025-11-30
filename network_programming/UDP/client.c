#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define SERVER_PORT 9090

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server_ip> <message> <repeat_count>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    const char *message = argv[2];
    int repeat = atoi(argv[3]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); exit(EXIT_FAILURE); }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    char buffer[BUF_SIZE];
    for (int i = 0; i < repeat; i++) {
        sendto(sockfd, message, strlen(message), 0,
               (struct sockaddr*)&server_addr, sizeof(server_addr));

        socklen_t len = sizeof(server_addr);
        ssize_t n = recvfrom(sockfd, buffer, BUF_SIZE-1, 0,
                             (struct sockaddr*)&server_addr, &len);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Echoed: %s\n", buffer);
        }
    }

    close(sockfd);
    return 0;
}
