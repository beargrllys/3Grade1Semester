#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <fcntl.h>     // 파일을 다루기 위한 헤더파일이다. file control

int access(const char *pathname, int mode);
//존재한는 pathname에 디렉토리 파일에 read or write가 가능한지 확인 합니다.
//만약 pathname의 파일이 symbolic link라면  link를 타고 들어간 파일의 permission을 확인합니다.
//본 체크는 오직 prrocessor의 real uid와 gid로 수행되어야 합니다. effective uid, gid로는 구동 불가
/*
    mode는 
    R_OK: 읽기 가능한지 / W_OK:쓰기 가능한지 / X_OK:실행 가능한지 / F_OK:파일이 존재하는지
    를 체크 할수 있고
*/
//성공시 0, 실패시 -1을 반환
// 체크한 항목들 중 하나라도 실패 하면 실패한것으로 보고 오류를 반환합니다.

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: a.out <pathname>");
        exit(1);
    }
    if (access(argv[1], R_OK) < 0)
    { //읽기 권한 가능여부 확인
        perror("access error");
    }
    else
        printf("read access OK\n");

    if (open(argv[1], O_RDONLY) < 0)
    { //open을 이용해 read가 가능한지 확인
        perror("open error");
    }
    else
        printf("open for reading OK\n");

    exit(0);
}
