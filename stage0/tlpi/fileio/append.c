/* The append program opens an existing file for writing with
 * the O_APPEND flag, and then seeks to the beginning of the file before
 * writing some data. */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

static void
usageError(char *progName)
{
    printf("Usage: %s filename [stuff to be written]\n", progName);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
    {
        usageError(argv[0]);
    }

    int opt;
    while ((opt = getopt(argc, argv, "h")) != -1)
    {
        switch (opt)
        {
            case 'h':
                usageError(argv[0]);
                break;
        }
    }

    int fd = open(argv[1], O_WRONLY | O_APPEND);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(fd, 0, SEEK_SET);
    if (offset == -1)
    {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    for (int i = optind + 1; i < argc; i++)
    {
        ssize_t numWritten = write(fd, argv[i], strlen(argv[i]));
        printf("%ld bytes written\n", numWritten);
        if (numWritten == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    int status = close(fd);
    if (status == -1)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
