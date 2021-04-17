//프로세서사 종료 될때 프로세서는 그 부모에세 종료 여부를 공지해야합니다.
//일반적으로는 exit()나 _exit()를 통해 exit_status 신호를 넘겨줍니다. exit_status는 최종적으로 _exit()함수가 호출될 때까지 종료상태로 변환하는 작업을 거칩니다.
//커널은 비정상 종료을 확인하기 위해 terminate status을 발생하기도 합니다.
// 종료된 프로세서의 부모는 wait()나 waitpid()같은 함수로 부터 terminate status를 획득합니다.
//init process는 모든 프로세스의 부모임으로 모든 프로세서는 init 프로세서에 의해 종료될수있다.
//pid와 CPU time에도 잡히지만 terminate status로 잡혀져있는 프로세스를 좀비상태라고 한다.
//즉, 프로세서는 종료된 상태이지만 부모프로세서가 기다리지 않으면 좀비 프로세서라고 부르는 것이다.

#include <stdlib.h>
#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <sys/wait.h>  // 프로세서 wait 관련 구동을 관할하는 헤더파일입니다.
#include <stdio.h>
#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일

void exit(int status);
//일반적인 프로그램의 종료를 야기하며 부모에게 종료상태를 반환합니다.
//열린 파일을 닫고 flush(모든 임시 버퍼를 디스트에 씁니다.)합니다.
//본 함수를 호출한 함수를 atexit()에 등록한후 최종적으로 _exit()를 호출합니다.

int atexit(void (*function)(void));
// 프로그램 종료를 호출한 함수를 받아 등록합니다. 다른 파라미터는 없습니다.
//성공하면 0 실패하면 -1을 반환합니다.

void _exit(int status);
//어떠한 종료 작업도 수행하지 않고 즉시 프로세스를 종료합니다.
//Process control block을 제외한 모든 자원을 반납합니다.
//status 파라미터를 통해 부모프로세서에게 프로세서가 종료 상태에 진입했을을 알립니다.

pid_t wait(int *status);
//자식 프로세서가 종료되거나 현재 프로세서를 종료하라는 시그널이 접수되기전까지 현재 프로세서의 실행을 막습니다.
//만일 호출자가 여러개의 자식프로세서를 가진다면 그중 하나의 프로세서가 종료될 때까지만 기다립니다.
//여러 자식프로세서 중 하나가 종료된다면 종료된 자식프로세서의 pid을 반환받기 때문에 누가 종료 된건지 확인할수 있습니다.
//만일 status가 NULL이면 반환받은 status정보를 무시합니다.
//만일 NULL이 아니라면 반환 받은 status값을 저장합니다.
//16bit이하의 정보를 제공받습니다.
//만약 종료된 이유가 정상적인 호출에 의한 종료라면 8번~15번의 비트에다가 반환 정보가 저장됩니다.
// 만약 비정상 종료에 의해 종료된 것이라면 0번~7번 비트에 반환정보가 저장됩니다.

/*
저장(반환)된 status 값의 의미
WIFEXITED(status): 정상적인 절차에 의해 종료 되었습니다.
WEXITSTATUS(status): 정상적인 절차에 의해 종료 되었으며 status값을 반환하였습니다.
WIFSIGNALED(status): signal에 의해 종료되었습니다.
WTERMSIG(status): signal에 의해 종료되었되, 어떤 signal에 의해 종료 되었는지 signal number을 반환합니다.
*/

int main(void)
{
    pid_t pid;
    int status;

    if ((pid = fork()) == 0)
    {
        printf("I am a child\n");
        exit(52); // 자식 프로세서 가 종료 되었습니다.
    }

    pid = wait(&status); // 부모는 자식프로세서가 종료될 떄까지 기다리다가 종료 보고서를 반환 받습니다.
    printf("parent: child(pid = %d) is terminated with status WIFEXITED(%d)\n", pid, WIFEXITED(status));
    printf("parent: child(pid = %d) is terminated with status WEXITSTATUS(%d)\n", pid, WEXITSTATUS(status));
    printf("parent: child(pid = %d) is terminated with status WIFSIGNALED(%d)\n", pid, WIFSIGNALED(status));
    printf("parent: child(pid = %d) is terminated with status WTERMSIG(%d)\n", pid, WTERMSIG(status));
    return 0;

    /*
    출력
    parent: child(pid = 241) is terminated with status WIFEXITED(1)//정상 종료 여부만 확인
    parent: child(pid = 241) is terminated with status WEXITSTATUS(123)//종료의 status 상태 까지 반환받음
    parent: child(pid = 241) is terminated with status WIFSIGNALED(0)//signal에 의한 종료가 아님으로 0반환
    parent: child(pid = 241) is terminated with status WTERMSIG(0)//signal에 의한 종료가 아님으로 0반환
    */
}