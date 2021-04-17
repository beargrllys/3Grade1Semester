//Process Group ID는 프로세스가 사용하기 쉽게 그룹에 묶여있는 것이다.
//IPC에서 signal을 보낼때 프로세스의 그룹으로 유용하게 사용한다.
//만약 어떤 프로세스 pid가 process group id와 동일하면 그 프로세스는 해당 프로세스 그룹의 리더이다.

#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>
#include <stdlib.h>

pid_t getpgrp(void);
//현재 프로세스의 process group id를 반환합니다.

int setpgid(pid_t pid, pid_t pgid);
//pid 프로세스의 process group id를 pgid로 설정한다.

//환경변수는 프로세스 환경에서 NULL로 끝나는 문자열을 어떤 변수로서 교체하는 값이다.
// NAME=something0 의 형식이다.

char *getenv(const char *name);
//name에 해당하는 환경변수의 값을 검색하고 조회합니다.
//조회시 해당 변수의 값을 반환하고 실패시 NULL을 반환
int putenv_(const char *string);
//환경 변수 값을 추가하거나 변경합니다.
//string에는 "NAME=something0"의 형식을 가져야합니다.
//존재하지 않는 NAME이라면 환경 변수 리스트에 NAME을 추가합니다.
//이미 존재하는 NAME이라면 해당 value로 바꿔줍니다.
//성공하면 - 실패하면 -1을 반환합니다.

int main(int argc, char *argv[], char *envp[])
{
    int i;
    extern char **environ;

    printf("from argument envp\n"); //이런 식으로도 환경변수가 받아와지네??

    for (i = 0; envp[i]; i++)
        puts(envp[i]);

    printf("\nfrom global vribale environ\n");

    for (i = 0; environ[i]; i++)
        puts(environ[i]);

    printf("Home directory is %s\n", getenv("HOME")); //원래 HOME 값 출력

    putenv("HOME=/"); //"/"로 HOME 환경변수 변경

    printf("New Home directory is %s\n", getenv("HOME")); //'/' 출력

    putenv("HOME"); //NULL로 HOME 환경변수 변경

    printf("New Home directory is %s\n", getenv("HOME")); //NULL 출력

    return 0;
}
