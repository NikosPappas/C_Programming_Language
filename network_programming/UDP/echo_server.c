#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <threads.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define PORT 9090
#define MAX_CONCURRENT 5

sem_t conn_sem;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread-safe logging
void log_event(const char *format, ...) {
    va_list args;
    va_start(args, format);

    time_t now = time(NULL);
    struct tm t;
    localtime_r(&now, &t);
    char timestr[64];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &t);

    pthread_mutex_lock(&log_mutex);
    printf("[%s] ", timestr);
    vprintf(format, args);
    printf("\n");
    pthread_mutex_unlock(&log_mutex);

    va_end(args);
}

// Structure to pass client info to thread
typedef struct {
    int sockfd;
    struct sockaddr_in client_addr;
    char buffer[BUF_SIZE];
    ssize_t msg_len;
} client_data_t;

int handle_client(void *arg) {
    client_data_t *data = (client_data_t *)arg;

    // Echo message back
    sendto(data->sockfd, data->buffer, data->msg_len, 0,
           (struct sockaddr*)&data->client_addr, sizeof(data->client_addr));

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &data->client_addr.sin_addr, ip, sizeof(ip));
    log_event("Echoed message to %s:%d", ip, ntohs(data->client_addr.sin_port));

    free(data);
    sem_post(&conn_sem);
    return 0;
}

int main() {
    sem_init(&conn_sem, 0, MAX_CONCURRENT);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); exit(EXIT_FAILURE); }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); exit(EXIT_FAILURE);
    }

    log_event("UDP Echo Server listening on port %d", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        char buffer[BUF_SIZE];

        ssize_t n = recvfrom(sockfd, buffer, BUF_SIZE, 0,
                             (struct sockaddr*)&client_addr, &len);
        if (n < 0) continue;

        sem_wait(&conn_sem);

        client_data_t *data = malloc(sizeof(client_data_t));
        data->sockfd = sockfd;
        data->client_addr = client_addr;
        memcpy(data->buffer, buffer, n);
        data->msg_len = n;

        thrd_t tid;
        thrd_create(&tid, handle_client, data);
        thrd_detach(tid);
    }

    close(sockfd);
    sem_destroy(&conn_sem);
    return 0;
}
