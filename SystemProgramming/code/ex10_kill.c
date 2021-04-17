#include <sys/types.h>
#include <signal.h>
#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>
#include <stdlib.h>

int kill(pid_t pid, int sig);
//특정 프로세스 그룹이나 프로세스에게 어떠한 시그널도 보낼수 있습니다.
// pid가 양수라면 pid 개개인에게 시그널을 보낼수 있습니다.
// pid가 0이라면 같은 프로세스에 속해있는 모든 프로세스들에게 시그널을 보냅니다.
// pid가 -1인 상태에서 effective user id가 super user라면 sig는 시스템프로세스를 제외한 모든 프로세스에게 보내집니다.
// pid가 -1인 상태에서 effective user id가 nomal user라면 real user id와 effective user id가 동일한 모든 프로세스에게 시그널이 보내집니다.
// pid가 -1보다 작으면 pid의 절댓값과 동일한 process group id를 지닌 모든 프로세스에게 시그널이 보내집니다.

//만약 sig가 0이라면 어떠한 시그널도 보내지지 않지만 error checking은 지속한다.
//성공하면 0 실패하면 -1을 반환합니다.

int ntimes = 0;

int main()
{
    pid_t pid, ppid;
    void p_action(int), c_action(int);
    static struct sigaction pact, cact;

    pact.sa_handler = p_action;      // pact sigaction에 p_action 사용자 설정 함수를 설정합니다.
    sigaction(SIGUSR1, &pact, NULL); // SIGUSR1시그널이 들어올때 pact sigaction을 시행토록 합니다.

    switch (pid = fork())
    {
    case 1:
        perror("synchro");
        exit(1);

    case 0:                              // 자식 프로세스
        cact.sa_handler = c_action;      // 자식 프로세스는 c_action사용자 설정 함수를 설정합니다.
        sigaction(SIGUSR1, &cact, NULL); //SIGUSR1 시그널이 들어올때 pact sigaction을 시행토록 합니다.
        ppid = getppid();                // 부모 프로세스의 pid를 받아옵니다.
        for (;;)
        {
            sleep(1);
            kill(ppid, SUGUSR1); // 1초에 한번 부모 프로세스에 SUGUSR1시그널을 보냅니다.
            pause();
        }

    default:
        for (;;)
        {
            pause();
            sleep(1);
            kill(pid, SIGUSR1); // 1초에 한번 자식 프로세스에 SUGUSR1시그널을 보냅니다.
        }
    }

    /*
    부모 프로세스 와 자식프로세스가 번갈아 시그널을 받아 출력 받습니다.
    */
}