#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdlib.h>
#include <stdio.h>

int chdir(const char *path) int fchdir(int fd);
//현재 디렉토리에서 newpath 디렉토리로 이동한다.
//성공하면 - 실패하면 -1을 반환합니다.

char *getcwd(char *buf, size_t size);
//현재 있는 디렉토리를 문자열로서 buf버퍼에 저장해주고 size에 그 길이를 저장합니다.
// 만약에 디렉토리 문자열 길이가 buf보다 길면 NULL을 반환합니다.
//성공하면 buf의 포인터 실패하면 NULL을 반환합니다.

int main(void)
{
    char *ptr;

    if (chdir("./foo-d") < 0)
    { //디렉토리 이동
        perror("chdir failed");
        exit(1);
    }

    if ((ptr = getcwd(NULL, 0)) == NULL)
    { //현재 디렏토리 받아옴
        perror("getcwd failed");
        exit(1);
    }

    printf("cwd = %s\n", ptr); //경로 확인
    exit(0);
}