#include <stdlib.h>
#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>
#include <signal.h>
#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.

#define TRUE 1
#define FALSE 0
#define BELLS "\007\007\007"

//int pause(void);
//별도의 시그널이 들어올때까지 프로세스를 멈춥니다.
// 항상 -1을 반환합니다.

//unsigned int alarm(unsigned int seconds);
//SIGALRM 시그널이 전달되어질 타이머를 설정합니다.
//seconds는 초단위이며 0이면 새로운 알람이 설정되지 않습니다.
//이전에 설정되었던 알람은 취소됩니다.
//예약한 알람이 전달될때까지 남은 시간을 초단위로 반환합니다.
//0은 이전에 설정된 알람이 없음을 뜻합니다.

int alarm_flag = FALSE;

void setflag(int sig)
{
    alarm_flag = TRUE;
}

int main(int argc, char **argv)
{
    int nsecs, j;
    pid_t pid;
    static struct sigaction act;

    if (argc <= 2) //매개변수가 2개 이상이어야함
    {
        fprintf(stderr, "Usage: tml #seconds message\n");
        exit(1);
    }

    if ((nsecs = atoi(argv[1])) <= 0) // 첫번째 인수는 숫자임
    {
        fprintf(stderr, "tml : invalid time\n");
        exit(2);
    }

    switch (pid = fork())
    {
    case -1:
        perror("tml");
        exit(1);
    case 0:
        break;
    default:
        printf("tml processid %d\n", pid); // 부모 프로세스만 출력
        exit(0);
    }

    // 부모 자식 둘다 이후의 과정을 거치게 됨

    act.sa_handler = setflag;
    sigaction(SIGALRM, &act, NULL); // sigaction설정

    alarm(nsecs); // nsecs 초후 시그널 도착 예정 -> setflag 함수에 의해 alarm_flag 변수가 True가 됨

    pause(); // 멈춰! 일단 프로세스가 멈추어 터미널이 프로세스를 끝낸것으로 인식함 -> nsecs SIGALRM 시그널에 의해 재개됨

    if (alarm_flag == TRUE) // 바뀌어진 전역변수에 의해 10초 뒤 출력값 발생
    {
        printf(BELLS);
        for (j = 2; j < argc; j++)
            printf("%s ", argv[j]);
        printf("\n");
    }
    return 0;
}