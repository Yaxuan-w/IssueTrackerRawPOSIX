#include <stdio.h> 
#include <fcntl.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>

long long gettimens() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (long long)tp.tv_sec * 1000000000LL + tp.tv_nsec;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <buffer_size> <loop_count>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int buf_size = atoi(argv[1]);
    int loop_count = atoi(argv[2]);
    char *buffer = (char *)malloc(buf_size);
    if (!buffer) {
        perror("Failed to allocate buffer");
        exit(EXIT_FAILURE);
    }
    memset(buffer, 'A', buf_size);

    // Create a dummy socket
    int sockfd[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == -1) {
        perror("socketpair");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    long long start_time = gettimens();

    for (int i = 0; i < loop_count; i++) {
        if (send(sockfd[0], buffer, buf_size, 0) == -1) {
            perror("send");
            free(buffer);
            close(sockfd[0]);
            close(sockfd[1]);
            exit(EXIT_FAILURE);
        }
    }

    long long end_time = gettimens();
    long long total_time = end_time - start_time;
    long long average_time = total_time / loop_count;

    fprintf(stderr, "\nBuffer size %d bytes: %d send() calls, average time %lld ns\n", buf_size, loop_count, average_time);
    fflush(NULL);

    free(buffer);
    close(sockfd[0]);
    close(sockfd[1]);

    return 0;
}
