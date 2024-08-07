#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Native linux
// #define FIFO_NAME "/home/lind/lind_project/myfifo"
// RawPOSIX
#define FIFO_NAME "/myfifo"

typedef struct {
    char* message;
} thread_arg;

void *write_thread(void *arg) {
    thread_arg *msg = (thread_arg *)arg;
    int fd;

    // Open FIFO
    if ((fd = open(FIFO_NAME, O_WRONLY)) < 0) {
        printf("Open failed in write thread \n");
        fflush(stderr);
        pthread_exit(NULL);
    }

    // Write to FIFO
    write(fd, msg->message, strlen(msg->message)+1);
    printf("Message sent: %s\n", msg->message);
    fflush(stdout);

    close(fd);
    pthread_exit(NULL);
}

void *read_thread(void *arg) {
    char buf[1024];
    int fd;

    // Open FIFO
    if ((fd = open(FIFO_NAME, O_RDONLY)) < 0) {
        printf("Open failed in read thread \n");
        fflush(stderr);
        pthread_exit(NULL);
    }

    // Read from file
    read(fd, buf, 1024);
    printf("Message received: %s\n", buf);
    fflush(stdout);

    close(fd);
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s [send message]\n", argv[0]);
        fflush(stdout);
        exit(1);
    }

    pthread_t writer, reader;
    thread_arg t_arg;
    t_arg.message = argv[1];
    
    // For native linux only
    // if (mkfifo(FIFO_NAME, 0666) < 0) {
    //     printf("mkfifo failed\n");
    //     fflush(stderr);
    //     exit(1);
    // }

    if (pthread_create(&writer, NULL, write_thread, &t_arg) != 0) {
        printf("Create writer thread failed\n");
        fflush(stderr);
        exit(1);
    }

    if (pthread_create(&reader, NULL, read_thread, NULL) != 0) {
        printf("Create read thread failed\n");
        fflush(stderr);
        exit(1);
    }

    // Wait threads to finish
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    // Remove FIFO - native only
    // unlink(FIFO_NAME);

    return 0;
}
