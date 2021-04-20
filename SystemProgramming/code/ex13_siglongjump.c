#include <stdlib.h>
#include <setjmp.h> // sigsetjmp()와 siglongjmp()를 사용하기 위한 헤더파일입니다.
#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.

void abort(void);
//SIGABORT 시그널을 발생시켜 비정상 종료 일으킵니다.
//abort() 함수에 의해 프로그램이 종료 되면 모든 open stream이 close되고 디스크로 flush되어 집니다.
//만약 abort()함수가 무시되어지면 ISO C에서는 여전히 프로세스가 종료 되고 다른 버전에서는 호출자에게 값을 반환합니다.
// 반환값은 전혀 없습니다.

int sigsetjmp(sigjmp_buf env, int savemask);
//프로그램의 스택을 저장해둠으로서 현재 프로그램 위치와 시그널 마스크를 저장합니다.
void siglongjmp(sigjmp_buf env, int val);
//저장된 위치로 다시 되돌립니다.

//savemask가 0이 아니라면 env에 현재 스택과 시그널 마스크를 저장합니다.
//0이면 그냥 env에 현재 스택과 시그널 마스크를 저장합니다.
//그리고 siglongjmp()를 호출하여 저장했던 위치로 다시 로드합니다.

//저장 후 즉시 sigjmp_buf를 로드했다면 0을 반환하며, siglongjmp()를 통해 반환하면 0이 아닌 숫자가 반환됩니다.
//siglongjmp()는 반환값이 전혀없습니다.

static void sig_usr1(int), sig_alrm(int);
static sigjmp_buf jumpbuf;
static volatile sig_atomic_t canjump;

void pr_mask(const char *str)
{
    sigset_t sigset;
    if (sigprocmask(0, NULL, &sigset) < 0) //현재 마스크 변수를 sigset에 저장함
    {
        perror("sigprocmask error");
        exit(1);
    }

    printf("%s", str);

    if (sigismember(&sigset, SIGINT)) //마스크 되어있는 시그널을 확인함
        printf("SIGINT ");
    if (sigismember(&sigset, SIGQUIT))
        printf("SIGQUIT ");
    if (sigismember(&sigset, SIGUSR1))
        printf("SIGUSR1 ");
    if (sigismember(&sigset, SIGALRM))
        printf("SIGALRM ");

    printf("\n");
}

int main(void)
{
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR) // 시그널에 대한 행동 설정
    {
        perror("signal (SIGUSR1) error");
        exit(1);
    }
    if (signal(SIGALRM, sig_alrm) == SIG_ERR) // SIGALRM시그널이 들어오면 sig_alrm 함수에서 또확인
    {
        perror("signal (SIGALRM) error");
        exit(1);
    }

    pr_mask("starting main: ");
    if (sigsetjmp(jmpbuf, 1)) // 현재 마스크 값을 저장 해둠 -> siglondjmp이후 이 위치 로 돌아 오며 siglondjmp 호출로 인해 0이 아닌 숫자가 반환
    {
        pr_mask("ending main"); // 메인 함수 종료
        exit(0);
    }
    canjump = 1;
    for (;;)
        pause(); // 시그널이 들어올때까지 기다렸다가 들어오면 sig_usr1 함수실행
}

static void sig_usr1(int signo)
{
    time_t starttime;
    if (canjump == 0)
    {
        return;
    }

    pr_mask("starting sig_usr1: ");
    alarm(3); // 3초후 시그널 예약
    starttime = time(NULL);
    for (;;)
        if (time(NULL) > starttime + 5) //5초 기다림 이와중에 kill -USR1 pid 명령
            break;

    pr_mask("finishing sig_usr1: "); // kill -USR1 pid 명령으로 인해 마스크 된 값이 생김
    canjump = 0;

    siglondjmp(jmpbuf, 1); // 원래 대로 복원
}

static void sig_alrm(int signo)
{
    pr_mask("in sig_alrm: ");
    return;
}