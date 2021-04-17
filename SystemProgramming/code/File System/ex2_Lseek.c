#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>
#include <fcntl.h> // 파일을 다루기 위한 헤더파일이다. file control

off_t lseek(int fd, off_t offset, int whence);
//fd의 offset을 재위치 시킵니다.
// whence: SEEK_SET: 파일의 시작 지점부처 offset을 이동합니다.
// SEEK_CUR: 현재 파일 커서부터 offset만큼 이동합니다.
// SEEK_END: 파일의 끝자락으로 이동합니다.
//HOLE: 파일의 전체 길이 이상으로 offset을 이동하면 중간에 공간이 생겨버린다. 그 중간의 공간을 '\0'으로 채워진다.
//성공시 파일의 시작 지점으로부터 커서가 얼마나 먼지 이동한 위치값을 반환한다.

char buf1[] = "abcdefgjij";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
    int fd;

    if ((fd = creat("file.hole", 0604)) < 0)
    {
        perror("create error");
        exit(1);
    }

    if (write(fd, buf1, 10) != 10)
    { //파일 시작지점으로 부터 10만큼 문자열을 쓴다.
        perror("buf1 write error");
        exit(1);
    }

    if (lseek(fd, 40, SEEK_SET) == -1)
    { // SEEK_SET이므로 시작지점으로 부터 40만큼 이동합니다. 그래서 앞에 10만큼 문자를 넘어 중간에 30 만큼은 빈공간이 생긴다.
        perror("lseek error");
        exit(1);
    }
    // offset now = 40

    if (write(fd, buf2, 10) != 10)
    { // 파일끝지점부터 10만큼 다시 문자를 습니다. 중간에 Hole도 파일의 일부로 취급합니다.
        perror("buf2 write error");
        exit(1);
    }
    //offset now = 50

    exit(0);
}