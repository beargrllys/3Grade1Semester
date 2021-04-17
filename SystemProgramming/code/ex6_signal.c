#include <signal.h>
#include <stdlib.h>
#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>
#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.

//void (*signal(int signum, void (*handler)(int)))(int)
//signal handler를 유저가 직접 만들수도 있다.
//siganl 함수 원형을 이용해 특정 시그널 발생시 동작 행위를 규정할수 있다.
//다만 시그널이 유실되는 경우도 있다
//그래서 최근에 UNIX시스템에서 signalaction()으로 최근에 구현되었다.
//handler에는 특정 함수나 SIG_IGN, SIG_DFL 적용가능

//signum으로 특정 signal number을 지정할 수 있으며 하나의 Signal handler에 여러 signal을 적용할수 있다.
//Signal handler의 이전 값이나 SIG_ERR을 반환한다.

static void sig_usr(int signo)
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGUSR2)
        printf("received SIGUSR2\n");
    else
    {
        fprintf(stderr, "received signal %d\n", signo);
    }

    return;
}

int main(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
    {
        perror("can't catch SIGUSR1");
        exit(1);
    }

    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
    {
        perror("can't catch SIGUSR2");
        exit(1);
    }

    for (;;)
    {
        pause();
    }
}

//SIGUSR1의 경우 kill 명령어를 통해 시그널을 전달할수 있음
//kill -USR1 pid 를 하면 "received SIGUSR1" 출력
//kill -USR2 pid 를 하면 "received SIGUSR2" 출력