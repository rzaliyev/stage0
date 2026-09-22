/* The program verify_dup verifies that duplicated file descriptors share
 * a file offset value and open file status flags. */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd = open("tfile", O_RDWR | O_CREAT,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IRWXG);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int newfd = dup(fd);
    if (newfd == -1) {
        perror("dup");
        exit(EXIT_FAILURE);
    }

    char *str = "test input";
    ssize_t numWritten = write(fd, str, strlen(str));
    if (numWritten != (ssize_t)strlen(str))
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(fd, 0, SEEK_CUR);
    if (offset == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    off_t new_offset = lseek(newfd, 0, SEEK_CUR);
    if (new_offset == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    printf("offest: %ld\n", offset);
    if (offset != new_offset) {
        printf("file offset of duplicated file descriptor differs from the original.\n");
    }

    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    int new_flags = fcntl(fd, F_GETFL);
    if (new_flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    int accessMode = flags & O_ACCMODE;
    printf("access mode: %x\n", accessMode);

    printf("flags: %x\n", flags);
    if (flags != new_flags) {
        printf("file status flags of duplicated file descriptor differs from the original.\n");
    }


    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    if (close(newfd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
