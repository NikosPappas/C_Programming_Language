#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <threads.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <regex.h>
#include <time.h>
#include <pthread.h>
#incldue <stdarg.h>

#define PORT 8080
#define BACKLOG 10
#define BUF_SIZE 4096
#define DOC_ROOT "./www"
#define THREAD_POOL_SIZE 4
#define MAX_QUEUE 16

// Logging
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
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

// Task queue for client sockets
typedef struct {
    int client_sockets[MAX_QUEUE];
    int front, rear, count;
    mtx_t mutex;
    sem_t sem_items;
    sem_t sem_slots;
} task_queue_t;

void queue_init(task_queue_t *q) {
    q->front = q->rear = q->count = 0;
    mtx_init(&q->mutex, mtx_plain);
    sem_init(&q->sem_items, 0, 0);
    sem_init(&q->sem_slots, 0, MAX_QUEUE);
}

void enqueue(task_queue_t *q, int client_fd) {
    sem_wait(&q->sem_slots);
    mtx_lock(&q->mutex);
    q->client_sockets[q->rear] = client_fd;
    q->rear = (q->rear + 1) % MAX_QUEUE;
    q->count++;
    mtx_unlock(&q->mutex);
    sem_post(&q->sem_items);
}

int dequeue(task_queue_t *q) {
    sem_wait(&q->sem_items);
    mtx_lock(&q->mutex);
    int client_fd = q->client_sockets[q->front];
    q->front = (q->front + 1) % MAX_QUEUE;
    q->count--;
    mtx_unlock(&q->mutex);
    sem_post(&q->sem_slots);
    return client_fd;
}

// Handle each client: minimal HTTP GET support
void handle_client(int client_fd) {
    char buffer[BUF_SIZE];
    ssize_t n = read(client_fd, buffer, sizeof(buffer) - 1);
    if (n <= 0) { close(client_fd); return; }
    buffer[n] = '\0';

    regex_t regex;
    regcomp(&regex, "^GET[ \t]+(/[^ \t]*)[ \t]+HTTP/1\\.[01]", REG_EXTENDED);
    regmatch_t matches[2];
    char filepath[512];

    if (regexec(&regex, buffer, 2, matches, 0) == 0) {
        int len = matches[1].rm_eo - matches[1].rm_so;
        snprintf(filepath, sizeof(filepath), "%s%.*s", DOC_ROOT, len,
                 buffer + matches[1].rm_so);
        if (filepath[strlen(filepath)-1] == '/') strcat(filepath, "index.html");
        log_event("Client requested: %s", filepath);
    } else {
        write(client_fd, "HTTP/1.1 400 Bad Request\r\n\r\n", 28);
        close(client_fd);
        regfree(&regex);
        return;
    }
    regfree(&regex);

    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        write(client_fd, "HTTP/1.1 404 Not Found\r\n\r\n", 27);
        log_event("File not found: %s", filepath);
    } else {
        write(client_fd, "HTTP/1.1 200 OK\r\n\r\n", 19);
        while ((n = read(fd, buffer, BUF_SIZE)) > 0) {
            write(client_fd, buffer, n);
        }
        close(fd);
        log_event("File sent: %s", filepath);
    }
    close(client_fd);
}

// Worker thread function
int worker_thread(void *arg) {
    task_queue_t *queue = (task_queue_t*)arg;
    while (1) {
        int client_fd = dequeue(queue);
        handle_client(client_fd);
    }
    return 0;
}

int main() {
    // Create document root if missing
    mkdir(DOC_ROOT, 0755);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(EXIT_FAILURE); }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(EXIT_FAILURE);
    }
    if (listen(server_fd, BACKLOG) < 0) { perror("listen"); exit(EXIT_FAILURE); }

    log_event("Thread Pool HTTP Server listening on port %d", PORT);

    // Initialize task queue
    task_queue_t queue;
    queue_init(&queue);

    // Start worker threads
    thrd_t workers[THREAD_POOL_SIZE];
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        thrd_create(&workers[i], worker_thread, &queue);
        thrd_detach(workers[i]);
    }

    // Accept loop
    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) continue;
        enqueue(&queue, client_fd);
    }

    close(server_fd);
    return 0;
}
