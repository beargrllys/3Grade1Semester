#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
    struct stat statbuf;
    if (stat("foo", &statbuf) < 0)
    {
        perror("stat error for foo");
        exit(1);
    } /* turn on set-group-ID and turn off group-execute */

    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
    {
        perror("chmoderror for foo");
        exit(1);
    }
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
    {
        perror("chmod error for bar");
        exit(1);
    }

    exit(0);
}