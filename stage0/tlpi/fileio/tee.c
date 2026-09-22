/* tee.c
 *
 * The tee command reads its standard input until EOF, writing a copy of
 * the input to standard output and to the file named in its command-line
 * argument.
 * By default, tee overwrites any existing file. The -a command-line option
 * (tee -a file), causes tee to append text to the end of a file. */

#include <string.h>
#include <fcntl.h>
#include "error_functions.h"
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int fd;

    if (argc > 3 || argc < 2 ||
        (argc == 3 && strcmp(argv[1], "-a") != 0) ||
        (strcmp(argv[1], "--help") == 0))
    {
        usageErr("%s [-a] file\n", argv[0]);
    }

    const char *file = (argc == 2) ? argv[1] : argv[2];

    int flags = O_RDWR | O_CREAT;
    if (strcmp(argv[1], "-a") == 0)
    {
        flags |= O_APPEND;
    }

    fd = open(file, flags,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
              S_IROTH | S_IWOTH);
    if (fd == -1)
    {
        errExit("open");
    }

    ssize_t numRead, numWritten;

    char c;
    while ((numRead = read(STDIN_FILENO, &c, 1)))
    {
        if (numRead == -1)
        {
            errExit("read");
        }

        numWritten = write(STDOUT_FILENO, &c, 1);
        if (numWritten == -1)
        {
            errExit("write");
        }

        numWritten = write(fd, &c, 1);
        if (numWritten == -1)
        {
            errExit("write");
        }
    }

    if (close(fd) == -1)
    {
        errExit("close");
    }

    return EXIT_SUCCESS;
}
