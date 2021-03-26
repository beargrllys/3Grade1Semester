#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
    int fd;
    if ((fd = creat("file.hole", 0640)) < 0)
    { //읽고 쓸수 있는 형태로 파일 열람
        perror("creat error");
        exit(1);
    }

    if (write(fd, buf1, 10) != 10)
    { //buf1 쓰기
        perror("buf1 write error");
        exit(1);
    }

    if (lseek(fd, 40, SEEK_SET) == -1)
    { //offset의 처음부터 40만큼 이동
        perror("lseek error");
        exit(1);
    }

    if (write(fd, buf2, 10) != 10)
    { // 40에서 10만큼 이동하여 50 위치에 위치
        perror("buf2 write error");
        exit(1);
    }
    /*offset result = 50*/
    exit(0);
}