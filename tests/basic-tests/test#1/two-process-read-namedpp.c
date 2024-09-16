#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

// Native linux only
// #define FIFO_NAME "/home/lind/lind_project/myfifo"
// RawPOSIx
#define FIFO_NAME "/myfifo"

int main(int argc, char **argv) {
    if (argc != 1) {
        printf("Usage: %s\n", argv[0]);
        fflush(stderr);
        exit(1);
    }

    int fd;
    char msg[1024];

    if ((fd = open(FIFO_NAME, O_RDONLY)) < 0) {
        perror("Open failed");
        exit(1);
    }

    ssize_t bytes_read = read(fd, msg, sizeof(msg) - 1);
    if (bytes_read < 0) {
        perror("Read failed");
        exit(1);
    }

    msg[bytes_read] = '\0';  // Null-terminate the string
    printf("Message read: %s\n", msg);
    fflush(stdout);

    close(fd);
    return 0;
}