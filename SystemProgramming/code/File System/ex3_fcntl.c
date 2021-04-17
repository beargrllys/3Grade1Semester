#include <stdio.h>
#include <fcntl.h>     // 파일을 다루기 위한 헤더파일이다. file control
#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.

int remove(const char *pathname);
//System call이 아닌 C라이브러리 함수이다.
//파일의 이름과 관련된 데이터를 말소합니다.
//file 형식은 unlink() System call을 부르고 디렉토리 현식은 rmdir() System call을 호출합니다.
//성공시 0을 실패시 -1을 반환합니다.

int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd, int cmd, struct lock *ldata);
//fd를 조작합니다. 이미 열린파일의 속성을 변경합니다.
//fd와 관련된 잡다한 동작을 수행합니다.
// 다양한 command cmd를 통해 동직을 수행합니다.
//cmd 속성: F_GETFL => fd의 flags를 받아옵니다. 모든 flag들이 반환될겁니다.
//cmd 속성: F_SETFL => 매개변수의 속성을 fd에 속성을 변경해 줍니다. 변경외 다른 속성을 영향을 받지 않습니다.
//성공시 0 실패시 -1을 반환합니다.

void set_fl(int fd, int flags)
{ //flags변수로 들어오는 속성값을 기존 val속성에 덮어 씌워준다.
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)))
    {
        perror("fcntl F_GETFL error");
        exit(1);
    }

    val |= flags; //비트 속성 수정
    if (fcntl(fd, F_SETFL, val) < 0)
    { // F_SETFL로 수정된 값 fd에 쓰기
        perror("fcntl F_SETFL error");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    int accmode, val;

    if (argc != 2)
    {
        fprintf(stderr, "usage:a.out <descript#>");
        exit(1);
    }

    if ((val = fctl(atoi(argv[1]), F_GETFL, 0)) < 0)
    { // fd의 속성을 받아옵니다.
        perror("fcntl error for fd");
        exit(1);
    }

    accmode = val & O_ACCMODE; // Access mode에 대한 값을 비트연산을 통해 걸러 냅니다.

    if (accmode == O_RDONLY) //어떤 속성값을 가지는 지 확인합니다.
        printf("read only");
    else if (accmode == O_WRONLY)
        printf("write only");
    else if (accmode == O_RDWR)
        printf("read write");
    else
    {
        fprintf(stderr, "unknown access mode");
        exit(1);
    }

    if (val & O_APPEND) //전체 속성을 가지는 값의 다른 속성을 확인합니다.  이외에도 다양한 속성값이 존재 합니다.
        printf(",append");
    if (val & O_NONBLOCK)
        printf(",nonblocking");
    if (val & O_SYNC)
        printf(, syschronous writes);
    putchar('\n');
    exit(0);
}
