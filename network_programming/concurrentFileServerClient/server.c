#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>          // file operations
#include <arpa/inet.h>
#include <netinet/in.h>
#include <threads.h>        // C11 threads
#include <semaphore.h>      // counting semaphore
#include <time.h>
#include <errno.h>
#include <stdarg.h>
#include <pthread.h>        // for log mutex

#define PORT 8080
#define BACKLOG 5
#define BUF_SIZE 1024
#define MAX_CONNECTIONS 3  // maximum simultaneous clients

// Global semaphore and log mutex
sem_t conn_sem;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread-safe logging with timestamp
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

// Thread function to handle a single client
int handle_client(void *arg) {
    int client_fd = *(int*)arg;
    free(arg);

    char buffer[BUF_SIZE];
    ssize_t n;

    // Receive requested filename
    n = read(client_fd, buffer, BUF_SIZE - 1);
    if (n <= 0) {
        close(client_fd);
        sem_post(&conn_sem);
        return 0;
    }
    buffer[n] = '\0';
    log_event("Client requested file: %s", buffer);

    // Open the requested file
    int fd = open(buffer, O_RDONLY);
    if (fd < 0) {
        snprintf(buffer, BUF_SIZE, "Error: File not found or cannot open\n");
        write(client_fd, buffer, strlen(buffer));
        close(client_fd);
        sem_post(&conn_sem);
        log_event("Failed to open file for client");
        return 0;
    }

    // Send file contents
    while ((n = read(fd, buffer, BUF_SIZE)) > 0) {
        if (write(client_fd, buffer, n) < 0) {
            perror("write");
            break;
        }
    }

    close(fd);
    close(client_fd);
    log_event("File transfer completed for client");

    // Release semaphore slot
    sem_post(&conn_sem);

    return 0;
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr;

    // Initialize semaphore
    sem_init(&conn_sem, 0, MAX_CONNECTIONS);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    log_event("Server listening on port %d", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        // Wait for available connection slot
        sem_wait(&conn_sem);

        int *client_fd = malloc(sizeof(int));
        if (!client_fd) {
            perror("malloc");
            sem_post(&conn_sem);
            continue;
        }

        *client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (*client_fd < 0) {
            perror("accept");
            free(client_fd);
            sem_post(&conn_sem);
            continue;
        }

        log_event("New client connected");

        // Spawn thread to handle client
        thrd_t tid;
        if (thrd_create(&tid, handle_client, client_fd) != thrd_success) {
            perror("thrd_create");
            close(*client_fd);
            free(client_fd);
            sem_post(&conn_sem);
        } else {
            thrd_detach(tid); // automatically release resources
        }
    }

    close(server_fd);
    sem_destroy(&conn_sem);
    return 0;
}
