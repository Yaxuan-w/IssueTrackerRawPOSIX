#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <sys/mman.h>

#define GB (1 << 30)

long long gettimens() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (long long)tp.tv_sec * 1000000000LL + tp.tv_nsec;
}

void parent(int socket, int buf_size, sem_t *semaphore) {
    char *send_buf = (char *)malloc(buf_size);
    char *recv_buf = (char *)malloc(buf_size);

    memset(send_buf, 'a', buf_size);
    
    sem_wait(semaphore);

    fprintf(stderr, "Starts sending: %lld\n", gettimens());
    fflush(stderr);

    for (int i = 0; i < GB / buf_size; ++i) {
        if (send(socket, send_buf, buf_size, 0) == -1) {
            perror("Send");
            exit(1);
        }
    }
    for (int j = 0; j < GB / buf_size; ++j) {
        int total_received = 0;
        while (total_received < buf_size) {
            int received = recv(socket, recv_buf + total_received, buf_size - total_received, 0);
            if (received == -1) {
                perror("Recv");
                exit(1);
            }
            total_received += received;
        }
    }

    

    fprintf(stderr, "Ends receiving: %lld\n", gettimens());
    fflush(stderr);

    free(send_buf);
    free(recv_buf);
}

void child(int socket, int buf_size, sem_t *semaphore) {
    char *send_buf = (char *)malloc(buf_size);
    char *recv_buf = (char *)malloc(buf_size);

    memset(send_buf, 'b', buf_size);

    if (sem_post(semaphore) < 0) {
        perror("sem_post");
        exit(1);
    }

    for (int x = 0; x < GB / buf_size; ++x) {
        int total_received = 0;
        while (total_received < buf_size) {
            int received = recv(socket, recv_buf + total_received, buf_size - total_received, 0);
            if (received == -1)
            {
                perror("Recv");
                exit(1);
            }
            total_received += received;
        }
    }
    for (int y = 0; y < GB / buf_size; ++y) {
        if (send(socket, send_buf, buf_size, 0) == -1) {
            perror("Send");
            exit(1);
        }
    }
    free(send_buf);
    free(recv_buf);
}

int main(int argc, char *argv[]) {
    int buf_size = 1 << 16;

    int sockets[2];
    pid_t pid;

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == -1) {
        perror("Socketpair");
        exit(1);
    }

    sem_t *semaphore = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (sem_init(semaphore, 1, 0) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("Fork");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        close(sockets[1]);
        child(sockets[0], buf_size, semaphore);
        close(sockets[0]);
    } else {
        // Parent process
        close(sockets[0]);
        parent(sockets[1], buf_size, semaphore);
        close(sockets[1]);
    }

    sem_destroy(semaphore);

    return 0;
}
