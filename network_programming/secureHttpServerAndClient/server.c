#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <regex.h>
#include <threads.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#define PORT 4433
#define BACKLOG 5
#define BUF_SIZE 4096
#define DOC_ROOT "./www"
#define MAX_CONNECTIONS 5

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

// Initialize SSL context
SSL_CTX* init_server_ctx() {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();
    method = TLS_server_method();
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

// Load certificate and key
void configure_server_ctx(SSL_CTX *ctx, const char *cert_file, const char *key_file) {
    if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

// Create TCP listening socket
int create_listen_socket(int port) {
    int sockfd;
    struct sockaddr_in addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(EXIT_FAILURE); }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(EXIT_FAILURE);
    }

    if (listen(sockfd, BACKLOG) < 0) { perror("listen"); exit(EXIT_FAILURE); }

    return sockfd;
}

// Thread function to handle each client
int handle_client(void *arg) {
    SSL *ssl = (SSL*)arg;
    char buffer[BUF_SIZE];
    ssize_t n;

    // Receive HTTP request
    n = SSL_read(ssl, buffer, sizeof(buffer) - 1);
    if (n <= 0) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
        sem_post(&conn_sem);
        return 0;
    }
    buffer[n] = '\0';

    // Parse GET request using regex
    regex_t regex;
    regcomp(&regex, "^GET[ \t]+(/[^ \t]*)[ \t]+HTTP/1\\.[01]", REG_EXTENDED);
    regmatch_t matches[2];

    char filepath[512];
    if (regexec(&regex, buffer, 2, matches, 0) == 0) {
        int len = matches[1].rm_eo - matches[1].rm_so;
        snprintf(filepath, sizeof(filepath), "%s%.*s", DOC_ROOT, len,
                 buffer + matches[1].rm_so);
        if (filepath[strlen(filepath) - 1] == '/') strcat(filepath, "index.html");
        log_event("Client requested file: %s", filepath);
    } else {
        SSL_write(ssl, "HTTP/1.1 400 Bad Request\r\n\r\n", 28);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        regfree(&regex);
        sem_post(&conn_sem);
        return 0;
    }
    regfree(&regex);

    // Open and send file
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        SSL_write(ssl, "HTTP/1.1 404 Not Found\r\n\r\n", 27);
        log_event("File not found: %s", filepath);
    } else {
        SSL_write(ssl, "HTTP/1.1 200 OK\r\n\r\n", 19);
        while ((n = read(fd, buffer, BUF_SIZE)) > 0) {
            SSL_write(ssl, buffer, n);
        }
        close(fd);
        log_event("File transfer completed: %s", filepath);
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);

    sem_post(&conn_sem);
    return 0;
}

int main() {
    // Initialize semaphore
    sem_init(&conn_sem, 0, MAX_CONNECTIONS);

    // Initialize SSL
    SSL_CTX *ctx = init_server_ctx();
    configure_server_ctx(ctx, "server.crt", "server.key");

    int sockfd = create_listen_socket(PORT);
    log_event("HTTPS server listening on port %d", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);

        sem_wait(&conn_sem); // limit concurrent clients

        int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &len);
        if (client_fd < 0) {
            perror("accept");
            sem_post(&conn_sem);
            continue;
        }

        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_fd);
        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            SSL_free(ssl);
            close(client_fd);
            sem_post(&conn_sem);
            continue;
        }

        thrd_t tid;
        thrd_create(&tid, handle_client, ssl);
        thrd_detach(tid);
    }

    close(sockfd);
    SSL_CTX_free(ctx);
    sem_destroy(&conn_sem);
    return 0;
}
