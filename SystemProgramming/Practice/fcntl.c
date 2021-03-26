/*#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd, int cmd, struct lock *ldata);*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int accmode, val;

    if (argc != 2)
    {
        fprintf(stderr, "usage:a.out <descriptor#>");
        exit(1);
    }

    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) //파일 fd1의 속성을 읽어 옵니다.
    {
        perror("fcntl error for fd");
        exit(1);
    }

    accmode = val & O_ACCMODE; //해당 속성의 마스트를 씌웁니다.

    if (accmode == O_RDONLY) //해당 파일이 어떤 속성인지 확인해봅니다.
        printf("read only");
    else if (accmode == O_WRONLY)
        printf("write only");
    else if (accmode == O_RDWR)
        printf("read write");
    else
    {
        fprintf(stderr, "unknown access mode");
        exit(1);
    }

    if (val & O_APPEND)
        printf(", append");
    if (val & O_NONBLOCK)
        printf(", nonblocking");
    if (val & O_SYNC)
        printf(". syschronous writes");
    putchar('\n');
    exit(0);
}