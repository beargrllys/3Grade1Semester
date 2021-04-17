#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <sys/stat.h>  //파일의 정보를 언디 위한 헤더 파일이다.
#include <stdio.h>
#include <stdlib.h>

int chmod(const char *path, mode_t mode);
int fchmod(int fd, mode_t mode);
//파일의 권한을 바꿉니다.
//주어진 디렉토리 문자열로 파일을 특정하거나 fd를 이용해 이용권한을 바꿉니다.
//mode는 S_I{R,W,X}{USR,GRP,OTH}으로 설정 가능합니다.
//성공시 0을 실패시 -1을 반환합니다.

int main(void)
{
    struct stat statbuf;

    if (stat("foo", &statbuf) < 0)
    { //현재 foo 파일의 stat를 받아옴
        perror("stat error for foo");
        exit(1);
    }

    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
    { //현재 그룹 아이디를 활성화하고(S_ISGID) 권한에 그룹 실행권한을 박탈한다(~S_IXGRP)
        perror("chmod error for foo");
        exit(1);
    }

    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
    { //bar파일의 실행권한을 -rw-r--r--로 변경한다.
        perror("chmod error for bar");
        exit(1);
    }

    exit(0);
}