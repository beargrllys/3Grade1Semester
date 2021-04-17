#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <sys/statvfs.h>
#include <stdio.h>
#include <stdlib.h>

void sync(void);
int fsync(int fd);
//파일에 메인메모리에 올라가있는 모든 데이터를 디스크에 내려서 최신버전임을 확인한다.
//일부 파일시스템에서 낮은수준의 파일 데이터 무결성을 유지 해야할때 이 프로그램을 호출한다.
//성공하면 - 실패하면 -1을 반환합니다.

int statvfs(const char *path, struct statvfs *buf);
int fstatvfs(int fd, struct statvfs *buf);
//디스크 단위의 파일시스템에 대한 정보얻는다.
//경로 이름이나 파일이 참조하는 파일시스템에 관한 정보를 반환
//statvfs 구조체를 반환한다.

int main()
{
    statvfs vfs;
    if (statvfs("./foo-d", &vfs))
    {
        printf("Uable to statfs\n");
        exit(1);
    }

    //이후  statvfs구조체 구조에 따라 값 출력/이용
}