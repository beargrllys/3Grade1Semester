#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.

int sigpending(sigset_t *set);
//블록되어버린 시그널을 set에 저장합니다
//성공하면 0 실패하면 -1을 반환합니다.

void fatal(const char *msg, int no)
{
    perror(msg);
    esit(no);
}

static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
    {
        fatal("can't reset SIGQUIT", 1);
    }
    return;
}

int main(void)
{
    sigset_t newmask, oldmask, pendmask;

    if (signal(SIGQUIT, sig_quit) == SIG_ERR) //SIGQUIT가 일어나면 sig_quit함수를 실행하도록 설정한다.
        fatal("can't catch SIGQUIT", 1);

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT); //새로운 mask에 SIGQUIT시그널을 저장한다.

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    { //oldmask에 이전상태를 저장하고  SIGQUIT를 블록시킨다.
        fatal("SIG_BLOCK error", 1);
    }
    sleep(5); // 이와중에 Ctrl+C를 눌러 SIGQUIT을 발생한다.

    if (sigpending(&pendmask) < 0) // 블록된 SIGQUIT가 저장됩니다.
        fatal("sigpending error");
    if (sigismember(&pendmask, SIGQUIT)) //pendmask에 SIGQUIT가 저장되었는지 확인합니다.
        printf("\nSIGQUIT pending\n");

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) // 다시 이전 블록 상태로 돌아갑니다.
        fatal("SIG_SETMASK error");
    printf("SIGQUIT unblocked\n");

    sleep(5); //이후 Ctrl+C를 누르면 프로세스가 종료됩니다.
    exit(0);
}