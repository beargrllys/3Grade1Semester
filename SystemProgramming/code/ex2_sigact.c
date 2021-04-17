#include <signal.h> //signal과 관련된 여러 변수와 함수를 담고 있는 헤더파일이다
#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>

int sigaction(int signum, const struct siganction *act, struct sigaction *oldact);

// 프로세스가 특정 시그널에 대하여 수행할 행동을 지정합니다.
//성공하면 0 실패하면 -1을 반환합니다.
//signum을 통해 특정 시그널을 지정합니다.SIGKILL과 SIGSTOP은 제외됩니다.
// 만일 act가 NULL이 아니라면 signum에 대한 새로운 Action이  act의 해당 시그널 action으로 저장됩니다.
// 만일 oldact가 NULL이 아니라면 이전까지 했던 Signal에 대한 행동이 저장됩니다.

//sigaction 구조체에는 여러가지 변수들이 존재하는데 다음과 같은 성질을 가진다.
//sa_handler에는 signum이 발생했을 때 실행할 handler의 포인터를 저장한다. 특이 함수 포이넡 외에 SIG_DFL, SIG_IGN이 지정 가능하다.
//sa_sigaction은 sa_flag가 SA_SIGINFO로 설정되었을때 추가적인 정보를 signal handler에게 전달한다.
//sa_mask는 SA_NOMASK가 적용되어있지 않은한 마스크 되어야할 시그널을 저장하고 마스크 시킵니다.
//sa_flags는 signal handling process에 적용되어야할 각종 옵션들을 제공합니다.
//SA_NOCLDSTOP: 자식프로세서 종료 시그널을 받지 않겠다.
//SA_RESETAND: 원래 불러와졌을때의 상태로 다시 복원하겠다.

void catchint(int signo)
{
    printf("\nCATCHINT: signo=%d\n", signo);
    printf("\nCATCHINT: returning\n\n");
    return;
}

int main()
{
    static struct sigaction act; // 새로운 sigaction 구조체를 선언합니다.
    act.sa_handler = SIG_IGN;    // 특정 시그널 발생시 catchint 함수를 실행할수 있도록 act구조체 내에 sa_handler를 설정합니다.
    //이때 catchint SIG_IGN로 바꾸면 시그얼이 무시 됩니다.

    sigfillset(&(act.sa_mask));    //sigaction 구조체가 모든 시그널을 받을수 있도록 모든 시그널을 열어줍니다.
    sigaction(SIGINT, &act, NULL); // SIGINT 시그널 발생시 catchint 함수를 수행하는 act sigaction을 반영해 줍니다.

    printf("Sleep call #1\n");
    sleep(1);

    printf("Sleep call #2\n");
    sleep(1);

    printf("Sleep call #3\n");
    sleep(1);

    printf("Sleep call #3\n");
    sleep(1);

    printf("Exiting\n");
    return 0;

    /*
    출력 
    cntl+C을 눌러 SIGINT 시그널을 발생시킬때마다 catchint의 출력 값이 발생한다.
    */

    //만약 기존의 sigaction을 저장했다가 복원하고 싶다면..

    struct sigaction oact;

    sigaction(SIGTERM, NULL, &oact); // 기존의 sigaction 저장

    sigaction(SIGTERM, &act, NULL); // 새로운 sigaction 반영

    /*여기서 다른 작업 후...*/

    sigaction(SIGTERM, &oact, NULL); // 기존의 sigaction 다시 복원
}