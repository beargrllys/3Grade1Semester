#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <sys/wait.h>  // 프로세서 wait 관련 구동을 관할하는 헤더파일입니다.
#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <fcntl.h>     // 파일을 다루기 위한 헤더파일이다. file control
#include <stdio.h>

extern char **environ;

int execl(const chat *pathname, const char *arg, void);
//p가 없어서 절대경로를 입력해주어야 한다.
//단순히 실행파일을 실행해준다. l이 존재함으로 매개변수에 직접 모든 옵션들을 적용해 줘야한다.

int execv(const char *pathname, char *const argv[]);
//v이 존재함으로 모든 옵션들은 배열에 넣어서 배열의 포인터로 적용해 줘야한다.

int execle(const char *pathname, const char *arg, char const *envp[]);
//e가 존재함으로 새로운 환경변수를 수동으로 넣어줄수 있다.
//l이 존재함으로 매개변수에 직접 모든 옵션들을 적용해 줘야한다.

int execve(const char *pathname, const char *arg[], char const *envp[]);
//v이 존재함으로 모든 옵션들은 배열에 넣어서 배열의 포인터로 적용해 줘야한다.

int execlp(const char *filename, const char *arg, void);
//p가 존재함으로 환경 변수가 자동적으로 적용된다 == 굳이 절대경로로 쓰지 않아도 된다. 기본적으로 usr/bin을 돌고 시작한다.
//l이 존재함으로 매개변수에 직접 모든 옵션들을 적용해 줘야한다.

int execvp(const char *filename, char const *argv[]);
//v이 존재함으로 모든 옵션들은 배열에 넣어서 배열의 포인터로 적용해 줘야한다.

//exec계열의 함수를 호출할때 해당 프로세스는 디스크에서부터 새로운 프로그램으로 완전히 대체된다.
//새로운 프로그램은 그것의 main함수에서 실행을 시작한다.
//위 여러가지 함수중 execve()만이 System call에 해당한다. 나머지는 execve()의 front-end function이다.
//매개변수를 통한 인수 작성에서는 마지막에 반드시 0을 넣어야하며 배열로 넘겨줄때는 배열의 마지막 원소는 반드시 NULL로 끝나야한다.
// 매개변수를 통한 인수작성에서는 처음에 절대경로나 경로를 넣고 그다음에 다시 본래 명령어를 넣어야한다.
// 리스트에서도 pathname에서는 절대 경로나 bin 명령어를 넣되, 인수 배열 1번째 원소는 명령어를 넣어야 한다.
/*
매개변수 양식 예시) execl("/bin/ls", "ls", "-al", 0);

배열 양식 예시) 
char *argv[3] = {"ls", "-al", 0};
execv("/bin/ls", argv);
*/

//환경변수를 넣어줄때에는 문자열 배열을 넣되, {"Title=Context",0}의 형태로 환경변수를 설정하여 넘겨주어야 한다.
/*
char *env[2] = {"TERM=vt100", 0};
execle("/bin/ls", "ls", 0, env);
*/

//exec로 실행했을때 부모로부터 물려 받는 것
/*
openfile(close-on-exec 설정에 따라 다르다)
pid,ppid, real-user(group)-id
process group id, session id, controlling terminal
working directory, root directory
환경변수(e자 돌림 exec 빼고)
*/

//exec로 실행했을때 부모로부터 물려 받지 않는 것
/*
Signal 받은 기록과 Signal set
*/

//성공시 반환값은 없으며, 에러시 -1 반환

int main(void)
{
    pid_t pid;
    int status;

    if ((pid = fork()) == 0)
    {
        execlp("ls", "ls", "-a", NULL); //ls 명령어가 실행된다.
    }
    else
    {
        wait(&status);
        printf("exit status of child = %d\n", WEXITSTATUS(status));
    }
}