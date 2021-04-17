#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <sys/wait.h>  // 프로세서 wait 관련 구동을 관할하는 헤더파일입니다.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일

pid_t waitpid(pid_t pid, int *status, int options);
// 본 함수는 특정 child process가 종료 될때까지 기다리는 함수입니다.
// pid 파라미터가 0보다 크면 정확이 pid와 동일한 자식프로세스가 종료될때까지 기다립니다.
// pid 파라미터가 -1이면 자식프로세스 중 하나라도 끝나길 기다립니다.
// pid 파라미터가 -1보다 작으면 프로세스 그룹ID가 pid의 절댓값과 같은 자식을 기다립니다.
// pid 파라미터가 0이면 waitpid()을 호출한 프로세스 그룹ID와 같은 프로세스 그룹ID를 가진 자식 프로세스를 기다립니다.
//option 파라미터는 기다리는 방식을 지정합니다. 3가지 옵션이 존재합니다.
//1. WCONTINUED: 본인 프로세스가 중단되었다가 다시 재개되면 자식프로세스의 상태정보를 받습니다.
//2. WNOHANG: 기다리는 pid가 종료되지 않았더라도 즉시 status값을 받아내어라
//3. WUNTRACED: 기다리는 pid가 멈춥상태 일때의 상태를 받아냅니다.
// 자식 프로세스가 중단되면 pid를 반환합니다.
// WNOHANG을 사용했거나 자식프로세서가 사용 불가상태라면 0이나 ECHILD를 반환받습니다.
//에러 발생시 -1을 반환받습니다.
// 여러 상황에서 status에 저장 되는 값은 다음과 같습니다.
//WIFSTOPPED(status): 현재 자식 프로세서가 멈춰있으면 true를 반환합니다.
//WSTOPSIG: WIFSTOPPED가 0이 아닌 상태에서 signal에 의해 멈취있다면 signal number를 반환받습니다.
//WCONTINUED: 본인 프로세스가 중단되었다가 다시 SICCONT에 의해 재개되면 자식프로세스의 상태정보를 받습니다.

int main()
{
    pid_t pid;
    int status, exit_status;

    if ((pid = fork()) < 0)
    {
        perror("fork Failed");
    }

    if (pid == 0)
    {
        printf("Child %d sleeping.. \n", getpid()); //4초간 자식프로세스 정지
        sleep(4);
        exit(5);
    }

    while (waitpid(pid, &status, WUNTRACED) == 0)
    {
        printf("Still waiting... \n");
        sleep(1);
    }

    if (WIFEXITED(status))
    {
        exit_status = WEXITSTATUS(status);
        printf("Exit status form %d was %d\n", pid, exit_status);
    }

    exit(0);

    /*
    출력 결과 
    WNOHANG: 자식이 뭘하든 status를 받아와서 still waiting... 먼저 실행하고 자식프로세스 출력값 출력
    WCONTINUED: 지식프로세스 동안 본인은 기다리기 때문에 still waiting...이 안나오고 바로 마무리 됨
    WUNTRACED: 자식이 중단되는 경우가 없어서 WCONTINUED 동일하지만 자식프로세서가 중단되는 시점이 있다면 그때 still waiting... 출력
    */
}