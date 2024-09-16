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
    if (argc != 2) {
        printf("Usage: %s [send message]\n", argv[0]);
        fflush(stderr);
        exit(1);
    }

    int fd;
    char *msg = argv[1];

    // Proper parenthesis to ensure correct assignment
    if ((fd = open(FIFO_NAME, O_WRONLY)) < 0) {
        perror("Open failed");
        exit(1);
    }

    if (write(fd, msg, strlen(msg)) < 0) {
        perror("Write failed");
        exit(1);
    }
    printf("Message Sent: %s\n", msg);
    fflush(stdout);

    close(fd);
    return 0;
}