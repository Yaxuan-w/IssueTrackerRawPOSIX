#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

// In native
//#define FIFO_NAME "/home/lind/lind_project/myfifo"
// In RawPOSIX
#define FIFO_NAME "/myfifo"

int main(int argc, char**argv)
{
    char buf[1024];
    int fd;
    pid_t pid;

    if (argc !=2 ) {
        printf("Usage : %s [send string]\n", argv[0]);
        exit(1);
    }

    // Only using in native linux
    /*
    if(mkfifo(FIFO_NAME, 0666) < 0) {
        printf("Failed to create fifo.\n");
        exit(1);
    }
    */

    pid = fork();

    if(pid < 0) {
        printf("Failed to fork.\n");
        exit(1);
    }

    if(pid > 0) {
        if((fd = open(FIFO_NAME, O_WRONLY)) < 0) {
            printf("Open failed.\n");
            exit(1);
        }

        write(fd, argv[1], strlen(argv[1])+1);
        printf("Send message : %s\n", argv[1]);

        close(fd);
        unlink(FIFO_NAME);
        exit(0);
    }

    if(pid == 0) {
        if((fd = open(FIFO_NAME, O_RDONLY)) < 0) {
            printf("Open failed.\n");
            exit(1);
        }
        read(fd, buf, 1024);
        printf("Receive message : %s\n", buf);

        close(fd);
        unlink(FIFO_NAME);
        exit(0);
    }

    return 0;
}
