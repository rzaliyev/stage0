/* The cp program that, when used to copy a regular file that containes
 * holes (sequences of null bytes), also creates corresponding holes in
 * the target file. */

#include <fcntl.h>
#include "tlpi_hdr.h"

int main (int argc, char *argv[])
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        usageErr("%s source_file dest_file", argv[0]);
    }

    int src_fd  = open(argv[1], O_RDONLY);
    if (src_fd == -1) errExit("open");

    int dst_fd  = open(argv[2], O_RDWR | O_CREAT,
                       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                       S_IROTH | S_IWOTH);
    if (dst_fd == -1) errExit("open");

    ssize_t numRead, numWritten;

    char c;
    bool inHole = false;

    while ((numRead = read(src_fd, &c, 1)))
    {
        if (numRead == -1) errExit("read");

        if (c == '\0')
        {
            inHole = true;
            continue;
        }

        if (inHole)
        {
            off_t currSrcOffset = lseek(src_fd, 0, SEEK_CUR);
            if (currSrcOffset == -1) errExit("lseek");
            off_t currDstOffset = lseek(dst_fd, 0, SEEK_CUR);
            if (currDstOffset == -1) errExit("lseek");

            if (currSrcOffset != currDstOffset)
            {
                currDstOffset = lseek(dst_fd, currSrcOffset - 1, SEEK_SET);
                if (currDstOffset == -1) errExit("lseek");
                inHole = false;
            }
        }

        numWritten = write(dst_fd, &c, 1);
        if (numWritten != 1) errExit("write");
    }

    if (close(src_fd) == -1) errExit("close");
    if (close(dst_fd) == -1) errExit("close");

    return EXIT_SUCCESS;
}
