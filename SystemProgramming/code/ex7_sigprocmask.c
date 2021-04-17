#include <signal.h>
#include <stdlib.h>
#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일

//Signal Mask는 시그널이 전달되는 것을 막습니다.
//블록된 시그널은 받는 프로세스가 안막혀서 처리하는데에 전적으로 달려있습니다.
// 시그널 마스트는 정수형 변수로 구현되며 각 비트가 하나의 시그널을 담당하여 비트가 1이면 해당 시그널이 블록되어 버립니다.
// 이런 시그널 마스크를 다루는 함수가 sigprocmask()함수 입니다.

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
// 블록된 시그널의 배열을 변경합니다.
//성공하면 0 실패하면 -1을 반환합니다.
//oldset NULL이 아니면 이전까지의 값이 oldset에 저장됩니다.
//sigprocmask로 SIGKILL과 SIGSTOP을 막을수 없습니다.
//set가 NULL 이라면 how가 무시되고 oldset에 현재값이 저장됩니다.
// how에 들어갈수 있는 값은 3가지 입니다.
//1. SIG_BLOCK: set에 존재하는 시그널들이 현재 블록된 값에 추가로 반영됩니다.
//2. SIG_UNBLOCK: set에 존재하는 시그널들이 현재 블록된 값에서 블록이 헤재됩니다.
//3. SIG_SETBLOCK: 모든 블록된 시그널을 해제하고 set에 존재하는 시그널을 블록합니다.

int main()
{
    sigset_t set1, set2;

    sigfillset(&set1); //set1에 모든 시그널을 채웁니다.

    sigfillset(&set2);
    sigdelset(&set2, SIGINT);
    sigdelset(&set2, SIGQUIT); //SIGINT와 SIGQUIT를 제외하고 모두 채워진 set2를 가공합니다.

    if (sigismember(&set2, SIGQUIT))
    { // sigset_t에 특정 시그널이 포함되어있는지 확인 합니다.
        printf("SIGQUIT시그널은 set2에 포함되어 있습니다.");
    }
    else
    {
        printf("SIGQUIT시그널은 set2에 포함되어 있지 않습니다."); //sigdelset(&set2, SIGQUIT);했으므로 본 문장이 읽힙니다.
    }

    sigprocmask(SIG_SETMASK, &set1, NULL); //현재 프로세스 signal handler에 set1의 모든 블록을 반영해 모든 시그널에 대한 블록을 처리합니다.

    sigprocmask(SIG_UNBLOCK, &set2, NULL); // set2에 SIGINT와 SIGQUIT를 제외한 모든 블록을 풉니다.

    sigprocmask(SIG_UNBLOCK, &set1, NULL); // set1의 모든 블록을 반영해 모든 블록을 풉니다.
}