/* Implement setenv() and unsetenv() using getenv(), putenv(), and, where necessary,
 * code that directly modifies environ. Your version of unsetenv() should check
 * to see whether there are multiple definitions of an environment variable, and
 * remove them all (which is what the glibc version of unsetenv() does. */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int setenv2(const char *name, const char *value, int overwrite);
int unsetenv2(const char *name);

int main(int argc, char *argv[])
{
    environ = NULL;

    if (setenv2("SPIDER", "MAN", 0) == -1) {
        exit(EXIT_FAILURE);
    }
    assert(strcmp(getenv("SPIDER"), "MAN") == 0);

    if (setenv2("SPIDER", "WOMAN", 0) == -1) {
        exit(EXIT_FAILURE);
    }
    assert(strcmp(getenv("SPIDER"), "MAN") == 0);

    if (setenv2("SPIDER", "WOMAN", 1) == -1) {
        exit(EXIT_FAILURE);
    }
    assert(strcmp(getenv("SPIDER"), "WOMAN") == 0);

    if (unsetenv2("SPIDER") == -1) {
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

int setenv2(const char *name, const char *value, int overwrite)
{
    int n = snprintf(NULL, 0, "%s=%s", name, value);
    if (n < 0) {
        return -1;
    }

    size_t size = (size_t)n + 1;
    char *buf = malloc(size);
    if (buf == NULL) {
        return -1;
    }

    n = snprintf(buf, size, "%s=%s", name, value);
    if ((size_t)n != size - 1) {
        free(buf);
        return -1;
    }

    if (!overwrite && getenv(name) != NULL) {
        free(buf);
        return 0;
    }

    if (putenv(buf) != 0) {
        free(buf);
        return -1;
    }

    return 0;
}

int unsetenv2(const char *name)
{
    if (name == NULL || strlen(name) == 0 || strchr(name, '=') != NULL) {
        errno = EINVAL;
        return -1;
    }

    int n = snprintf(NULL, 0, "%s=", name);
    if (n < 0) {
        return -1;
    }

    size_t size = (size_t)n + 1;
    char *buf = malloc(size);
    if (buf == NULL) {
        return -1;
    }

    n = snprintf(buf, size, "%s=", name);
    if ((size_t)n != size - 1) {
        free(buf);
        return -1;
    }

    char **ep;
    int i = 0;
    int j = 0;
    for (ep = environ; *ep != NULL; ep++) {
        if (strncmp(*ep, buf, strlen(buf)) == 0) {
            for
        }
        ++i;
    }


    return 0;
}
