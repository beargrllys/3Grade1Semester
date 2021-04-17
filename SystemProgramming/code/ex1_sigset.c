//signal은 software Interrupt로 process에 무언가 발생했음을 알리는 비동기적 메세지이다.
// 한 프로세스에서 생성해서 다른 프로세스로, 혹은 자기 자신에서 보낼수있다.
//비동기적 메세지에 프로세스를 종료하거나 무시할수 있다.
//signal에 대한 행동을 결정하는 것을 signal handling 이라고 한다. : Default 동작수행 / Ignored 무시 / user-define

//Signal이 발생하는 경우는 다음과 같다.
// 하드웨어에서 시그널
// 터미널에서 발생한 중지 시그널
// 프로세스 죽이는 kill 시그널
// timer 경과 시그널
// 자식 프로세서의 종료

// 시그널은 같은 user ID끼리의 Intraprocess, Interprocess, Kernel-process 간 시그널이 사용되어 소통한다.
// 시그널이 취할수 있는 동작은 다음과 같다.
//1. Generated: 어떤 이벤트로 인해 시그널이 발생함
//2. Delivered: 시그널이 목적지에 도착하였음
//3. Pending: 시그널이 발생해서 도착하기 까지의 시간
//4. Blocked: mask로 인해 시그널이 막혀버림

// 시그널을 받아들이는 방법
//1. Default action(SIG_DFL): 일반적인 시그널에 의해 동작 중지
//2. Ignored (SIG_IGN): 시그널이 먹히지 않게 막음
//3. User define Action: 유저가 정한 user define signal handler이나 signal catching function에 의해 동작
//SIGKILL이나 SIGSTOP을 제외한 대부분의 시그널이 잡히거나 무시당함

//대표적인 시그널의 종류
//SIGINT: 인터럽트 야기
//SIGQUIT: 중지
//SIGABORT: 강제 종료
//SIGUSR1: User Define Signal 1
//SIGUSR2: User Define Signal 2
//SIGPIPE: 파이프 폐기
//SIGALRM: 타이머 알람
//SIGTERM: 종료
//SIGSTOP: 정지(절대 시그널)
//SIGTSTP: 키보드에 의한 정지
//SIGPOLL: 오염될 이벤트 발생

//signal set은 시그널을 다루는 System Call에 넘겨줘야할 대표적인 파라미터 중 하나이다.
//여기에는 발생가능한 signal을 담고있는 세트이며 sigset_t라는 새로운 데이터 타입으로서 제공된다.
// 그리고 더불어 사전정의된 5개의 함수 또한 포함한다.

#include <signal.h> //signal과 관련된 여러 변수와 함수를 담고 있는 헤더파일이다
#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>

int sigemptyset(sigset_t *set);
// 아무것도 없는 빈 signal set을 초기화한다. 존재하던 모든 시그널이 set에서 소거된다.
//성공하면 0 실패하면 -1을 반환합니다.

int sigfillset(sigset_t *set);
// 모든 signal을 포함하여 초기화한다. 꽉찬 signal set이다.
//성공하면 0 실패하면 -1을 반환합니다.

int sigaddset(sigset_t *set, int signum);
//set에 signum의 시그널을 추가합니다.
//성공하면 0 실패하면 -1을 반환합니다.

int sigdelset(sigset_t *set, int signum);
//set에 signum의 시그널을 제거합니다.
//성공하면 0 실패하면 -1을 반환합니다.

int sigismember(const sigset_t *set, int signum);
//특정 시그널이 set에 포함되어 있는지 확인합니다.
// 1이면 이미 포함되어있는 것이고, 0이면 포함되지 않은것, -1이면 에러입니다.

int main()
{
    sigset_t mask1, mask2;

    sigemptyset(&mask1); // mask1을 초기화 합니다.

    sigaddset(&mask1, SIGINT); //mask1에 SIGINT 시그널을 추가합니다.

    sigaddset(&mask1, SIGQUIT); //mask1에 SIGQUIT 시그널을 추가합니다.

    sigfillset(&mask2); //모든 시그널을 포함한 sigset으로 mask2을 초기화합니다.

    sigdelset(&mask2, SIGCHILD); //mask2에서 SIGCHILD를 제거합니다.
}