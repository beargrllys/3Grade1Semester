#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BSIZE 80

int main(void)
{
    int fd, newfd, n;
    char buf1[BSIZE], buf2[BSIZE];

    fd = open("/etc/passwd", O_RDONLY);
    newfd = dup(fd);
    n = read(fd, buf1, BSIZE);

    printf("Read from fd:\n\n");
    write(STDOUT_FILENO, buf1, n);

    n = read(newfd, buf2, BSIZE);
    printf(“\n\nRead from newfd
           :\n\n”);
    write(STDOUT_FILENO, buf2, n);

    close(fd);

    n = read(newfd, buf1, BSIZE);
    printf("\n\nRead from newfd after close(fd):\n\n");
    write(STDOUT_FILENO, buf1, n);
    printf("\n");

    close(newfd);
    exit(0);
}