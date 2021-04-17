// UNIX시스템에서 shell은 프로그램을 실행할 때마다 새로운 프로세스를 생성해준다.
//프로세스는 프로그램을 실행시킨것이다.
//모든 프로세스는  fork/exec 모델을 이용해 다른 프로세스를 샐행시킬 수 있다.
//프로세스를 다루는 System call에는 fork(),exec(), wait(), exit()이 있다.
//fork(): 호출된 프로세스를 복제하여 새로운 프로세스를 만든다.
//exec(): 프로세스에 확보된 공간에 새로운 프로그램을 덧씌운다.
//wait(): 새오 생성된 프로세스와 동기화를 시켜준다, 다른 관련 프로세스가 완료될 때까지 프로세스를 기다려준다.
//exit(): 프로세스를 종료합니다.
//pid: process table entry에 있는 index를 뜻하며 프로세스 마다 고유한 값을 가진다.

#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>

pid_t getpid(void);
//현재 프로세스의 pid를 반환합니다.

pid_t getppid(void);
//부모프로세스의 pid를 반환합니다.

/*
LINUX SWAPPER PROCESS
- pid 0번에 할당하며 모든 프로세스의 조상이다.
- 커널에 필요한 모든 데이터 구조를 초기화하며 인터럽트를 활성화하고, init process하는 커널쓰레드를 생성합니다.
- init process을 생성하고 난후 cpu_idle()함수를 지속적으로 실행합니다.
- 다른 프로세스가 실행되는 것이 없을때 Swapper이 스커줄러에게 선택됩니다.

LINUX INIT PROCESS
- init함수는 execve() System call를 통해 /sbin/init 디렉토리에 있는 실행가능한 프로그램을 구동합니다.
- 결과적으로 init 쓰레드는 프로세스별 데이터 구조를 가지는 일반 프로세스가 됩니다.
- 즉, init()은 super user를 가지는 일반 프로세서라고 생각할수 있습니다.
//모든 user processor의 활동을 모니터링하고 생성하는 역할을 가지기에 절대 종료되지 않습니다.
*/

pid_t fork(void);
//새로운 프로세스를 만드는 유일한 방법은 기존에 존재하는 프로세스에서 fork() System call을 호출하는 것입니다.
//fork()에서 새로 만들어진 프로세스를 child process라고 부릅니다.
//fork()함수가 한번 호출되면 2개의 값이 반환됩니다. child process의 0과 parent process의 child process의 실제 pid
//fork() 호출 이후에는 child process와 parent process 둘다 같이 실행되게 됩니다.
//child process는 parent process의 복사본입니다
// child process은 부모의 data, heap, stack 공간을 복사하게 됩니다.
//종종 child process는 부모와 text segment를 공유하게 됩니다.
//Process내에 있는 U-Area에는 open files List와 현재 디렉토리에 대한 정보가 담겨있습니다.
//child process는 부모에게 다음과 같은 항목을 물려받습니다: open files, file mode, uid, gid, directory
//단, pid와 ppid는 부모와 자식이 서로 다릅니다.
// 일단 child process가 실행되면 부모보다 먼저 실행되는지 아닌지는 알수가 없다. 그래서 필요할 경우 따로 동기화 과정이 필요하다.
//먼저 실행될지에 대한 부분은 kernel단에 스케줄링 알고리즘에 따라 달려있다.
//부모 프로세서에 의해 열려진 모든 fd는 child process에도 fd가 복제 됩니다.
//fork이후에 fd를 통제 하기 위해서는 다음과 같은 방법이 있습니다.
//1. 부모프로세스가 자식프로세스가 부모의 fd를 필요치 않을때까지 자식프로세스를 기다립니다.
// 만약 자식프로세가 종료 되어지면 그동안 자식프로세서가 fd에 일고 쓴 것들을 기존fd에 반영합니다.
//2.부모 프로세서와 자식 프로세서가 각자 갈길을 가는 방법
//fork이후에 부모와 자식 프로세서가 각자 자신에게 필요없는 fd를 닫습니다. 그래서 서로 다른 fd에게 어떤일이 일어나는지 확인할수 없습니다.
//Fork() 함수가 실패하는 데에는 크게 2가지 이유가 있습니다.
//1. 시스템에 이미 너무 많은 프로세스가 존재해서
//2. 한 real user id에 할당된 프로세스의 개수가 시스템의 한계 개수를 초과했을때
//fork()의 사용은 크게 2가지 방법으로 사용됩니다.
//1. 프로세스가 자체적으로 복사하여 코드에 서로다른 부분을 맡아 실행하고 싶을때
//2. 프로세스가 와전 다른 프로그램을 실행시키고 싶을때 exec()를 이용해 새로운 프로그램을 실행시킨다.

void main(void)
{

    pid_t pid;

    printf("Hello, my pid is %d\n, get pid()");

    if ((pid = fork()) == 0)
    { //자식식님 프로세스 시행 구간
        printf("child: pid = %d, ppid = %d", getpid(), getppid());
    }
    else
    { //부모프로세스 실행구간
        printf("parent: I created child with pid=%d\n", pid);
    }

    printf("Bye, my pid is %d\n", getpid());
}