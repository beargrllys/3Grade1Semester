#include <stdio.h>
#include <fcntl.h>     // 파일을 다루기 위한 헤더파일이다. file control
#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdlib.h>

#define BSIZE 80

int dup(int oldfd);
int dup2(int oldfd, int newfd);
//받아온 fd를 복사해 새로운 fd나 지정된fd로 복사한다.
//두 fd는 서로 교환되어 사용되어질수있다.
// 만약 한 fd가 lseek에 의해 위치가 바뀔경우 다른 lseek도 위치가 덩달아 바뀐다.
// dup()함수는 새로은 fd를 확보하기 위해 사용되지 않는 가장 낮은 번호의fd에 복사해준다.
// dup2()함수는 newfd에 oldfd를 복사하고 new fd를 닫아준다. 조금 올드한느낌이다.
// 성공할 경우 새로 만들어진 fd를 실패할경우 -1을 반환한다.

int main()
{
    int fd, newfd, n;
    char buf1[BSIZE], buf2[BSIZE];

    fd = open("/etc/passwd", O_RDONLY); // 해당 디렉토리의 fd 번호를 받아온다.
    newfd = dup(fd);                    //위의 fd를 복사한 새로운 fd를 만든다.

    n = read(fd, buf1, BSIZE); //fd에 있는 데이터를 buf버퍼에다가 읽는다.
    printf("Read from fd: \n\n");
    write(STDOUT_FILENO, buf1, n); //표준 출력 fd에 buf1을 쓴다.

    /*
    출력: 
    root:x:0:0:root:/root:/bin/bash
    daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
    //무난한 처음부터 버퍼크기 만큼 읽어옴
    */

    n = read(newfd, buf2, BSIZE); //newfd에 있는 데이터를 buf버퍼에다가 읽는다.
    printf("\n\nRead from newfd:\n\n");
    write(STDOUT_FILENO, buf2, n); //표준 출력 newfd에 buf1을 쓴다.

    /*
    출력: 
    bin:x:2:2:bin:/bin:/usr/sbin/nologin
    sys:x:3:3:sys:/dev:/usr/sbin/nologin
    //fd와 newfd가 같은 file table에 같은 파일 속성값을 가리키면서 offset이 서로다른 fd에서 조정되었음에도 불구하고 
    이어서 출력되고잇다.
    */

    close(fd);

    n = read(newfd, buf1, BSIZE); //기존 fd를 닫고 난후 newfd에 있는 데이터를 buf1에 읽는다.
    printf("\n\nRead from newfd after close(fd):\n\n");
    write(STDOUT_FILENO, buf1, n); // buf1을 출력해본다.
    printf("\n");

    /*
    출력: 
    :4:65534:sync:/bin:/bin/sync
    games:x:5:60:games:/usr/games:/usr/sbin/nologin

    한쪽 fd를 닫고 나서 출력해봐도 file table이 사라진것은 아니기에 이어서 출력 된다.
    */

    close(newfd);
    exit(0);
}