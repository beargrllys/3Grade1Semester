//전통적인 UNIX시스템에서 부모프로세스가 자식프로세스에게 곧이 곧대로 복사하는 것은 너무 느리고 비효율적임
// 그래서 이러한 문제를 해결하기 위한 3가지 방법
//1. Copy on write
// 일단 child process와 parent process가 같은 pysical page를 사용하게 하다가 누군가 해당 pysical page를 건들이면 그때 새로운 pysical page를 복제하는 방법
//2. Lightweight process
//부모와 자식이 최대한 open file table, paging table 같은 기본적인 정보를 최대한 공유하는것
//3. vfork()
//부모가 무조건 자시을 기다리게 만들어서 굳이 제갈길 가게 만들 상황을 원천 차단하는 방법

#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>
#include <stdlib.h>

pid_t clone(void *child_stack, unsigned long flags);
//경량형 프로세스를 만듭니다.
// 호출한 프로세스와 호출된 프로세스가 어떤 자원을 공유할지를 지정해줍니다.
// 새로운 프로세스가 사용할 메모리 영역이 어딘지를 특정하게 지정해야합니다.
// 일반적인 프로그램에서 사용해서는 안되며 fork()나 pthread_create()함수 구현에 clone() System call을 사용하게 됩니다.

//자식프로세서가 사용할 stack의 포인터를 지정하기위해 Child_stack을 지정합니다.
// 만약 Child_stack 파라미터가 0이라면 부모의 stack을 사용합니다. 즉, 일시적으로 stack을 공유하는 건데, 결국 Copy on write 방식에 의해 스택에 수정을 가하면 새로운 스택을 복제할 것입니다.

//flag 파라미터에는 4byte 짜리 변수인데 앞에 1byte에는 부모가 자식에게 종료 명령을 내리는 신호를 보내주기 위해 일반적으로 SIGCHLD를 전해줍니다.
//나머지 3byte는 어떤 리소스를 공유할지 지정해주는 작업을 시행합니다. ex) CHILD_FILES: open file table을 공유하라

pid_t vfork(void);
//vfork는 fork와 비슷하지만 부모의 주소공간을 복사하지 않고 자식이 exec나 exit() 할 때까지 부모의 주소공간에서 작업을 실행하게 하고  작업이 끝나기 까지 부모 프로세스는 작업을 중단시킵니다.
// 그래서 child process가 exec()[exec하는 순간 이제 다른 프로그램을 실행해야 함]나 exit()를 호출하여 먼저 끝낼수 있도록 보장해줍니다.

int glob = 6;

int main(void)
{
    int var;
    pid_t pid;

    var = 88;
    printf("before vfork\n");

    if ((pid = vfork()) < 0)
    {
        perror("vfork error");
        exit(1);
    }

    else if (pid == 0) // 일단 자식프로세스 먼저 시행한후
    {
        glob++;
        var++;
        _exit(0); //프로세스를 닫는 마무리 작업을 하지 못하고 즉시 종료하겠다. child process에서  코드가 진행되어 출력값이 나오는 것을 막습니다.
    }

    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var); // 자식프로세스 종료 후 출력됨

    exit(0);

    /*
    위 코드 출력: pid = 204, glob = 7, var = 89

    fork()로 변경시 출력: 
    pid = 211, glob = 6, var = 88 //부모 프로세스가 값을 더해주는 작업을 건너뛰며 값이 달라짐
    pid = 212, glob = 7, var = 89 //자식 프로세스는 값이 더해짐

    _exit(0) 삭제시 출력:
    pid = 226, glob = 7, var = 89 //부모도 자식이 주소 공간을 공유해서 자식의 부모의 공간에 값을 바꾸기 때문에 값이 둘다 바뀌어 출력
    pid = 225, glob = 7, var = 89
    
    */
}