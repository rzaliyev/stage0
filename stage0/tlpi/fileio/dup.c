/* The dup() function duplicates file descriptor by creating new one
 * that refers to the same open file description. The dup() call
 * takes oldfd, an open file descriptor, and returns a new descriptor
 * that refers to the same open file description. The new descriptor is
 * guaranteed to be the lowest unused file descriptor.
 *
 * Function signature: int dup(int oldfd);
 *
 *
 * */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int my_dup(int oldfd);
int my_dup2(int oldfd, int newfd);

int main(int argc, char *argv[])
{
    int fd = open("tfile", O_RDWR | O_APPEND);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int newfd = my_dup2(fd, 10);
    if (newfd == -1) {
        perror("my_dup");
        exit(EXIT_FAILURE);
    }

    printf("oldfd: %d, newfd: %d\n", fd, newfd);

    const char *text = "the text";

    ssize_t written = write(newfd, text, strlen(text));
    if (written == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    if (fd != newfd && close(newfd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

int my_dup(int oldfd)
{
    int newfd = fcntl(oldfd, F_DUPFD, oldfd);
    return newfd;
}

int my_dup2(int oldfd, int newfd)
{
    if (oldfd == newfd) {
        if (fcntl(oldfd, F_GETFL) == -1) {
            errno = EBADF;
            return -1;
        } else {
            return newfd;
        }
    }

    if (fcntl(newfd, F_GETFL) != -1) {
        printf("closing exising fd: %d\n", newfd);
        close(newfd);
    }

    newfd = fcntl(oldfd, F_DUPFD, newfd);

    return newfd;
}
