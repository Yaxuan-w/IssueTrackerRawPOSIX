#include <stdio.h> 
#include <fcntl.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <time.h>
#include <string.h>

#define LOOP_COUNT 1000000

long long gettimens() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (long long)tp.tv_sec * 1000000000LL + tp.tv_nsec;
}

int main(int argc, char *argv[]) {
    // int buffer_sizes[] = {1, 16, 256, 4096, 65536}; // 2^0, 2^4, 2^8, 2^12, 2^16
    // int num_sizes = sizeof(buffer_sizes) / sizeof(buffer_sizes[0]);
    int buf_size = atoi(argv[1]);
    char *buffer = (char *)malloc(buf_size);
    if (!buffer) {
        perror("Failed to allocate buffer");
        exit(EXIT_FAILURE);
    }
    memset(buffer, 'A', buf_size);
    
    long long start_time = gettimens();
    
    for(int i = 0; i < LOOP_COUNT; i++) {
        write(STDOUT_FILENO, buffer, buf_size);
    }
    
    long long end_time = gettimens();
    long long total_time = end_time - start_time;
    long long average_time = total_time / LOOP_COUNT;
    
    fprintf(stderr, "\nBuffer size %d bytes: %d write() calls, average time %lld ns\n", buf_size, LOOP_COUNT, average_time);
    fflush(NULL);
    
    free(buffer);
    
    return 0;
}
