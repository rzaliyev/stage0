/* Program atomic_append should open the specified 'filename' (creating it if necessary)
 * and append 'num-bytes' to the file by using write() to write a byte at a time.
 * By default the O_APPEND flag is used, but can be ommited if 'x' argiment is used. */

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4) {
        printf("Usage: %s filename num-butes [x]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    bool isSeek = false;
    int flags = O_WRONLY | O_CREAT;
    if (argc == 4 && strcmp(argv[3], "x") == 0) {
        isSeek = true;
    } else {
        flags |= O_APPEND;
    }

    int fd = open(argv[1], flags,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWUSR |
                  S_IROTH | S_IWOTH);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    char *endptr;
    long val = strtol(argv[2], &endptr, 0);

    if (errno == ERANGE) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }

    if (endptr == argv[2]) {
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }

    char c = '\0';

    for (long i = 0; i < val; i++) {
        if (isSeek && (lseek(fd, 0, SEEK_END) == -1)) {
            perror("lseek");
            exit(EXIT_FAILURE);
        }

        int written = write(fd, &c, 1);
        if (written != 1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if (close(fd)) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
