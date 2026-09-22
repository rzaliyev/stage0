/* Implement getpwnam() using setpwnam(), getpwent(), and endpwent(). */

#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct passwd *getpwnam2(const char *name);

int main(int argc, char *argv[])
{
    struct passwd *pwd = getpwnam2("bakhtik");
    if (pwd != NULL) {
        printf("%s:%s:%d:%d:%s:%s:%s\n",
                pwd->pw_name,
                pwd->pw_passwd,
                pwd->pw_uid,
                pwd->pw_gid,
                pwd->pw_gecos,
                pwd->pw_dir,
                pwd->pw_shell);
    }
    return EXIT_SUCCESS;
}

struct passwd *getpwnam2(const char *name)
{
    struct passwd *pwd;

    while ((pwd = getpwent()) != NULL) {
        if (strcmp(pwd->pw_name, name) == 0) {
            setpwent();
            return pwd;
        }
    }

    endpwent();
    return NULL;
}
