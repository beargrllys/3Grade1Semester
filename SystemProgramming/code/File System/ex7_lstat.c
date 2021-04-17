#include <sys/stat.h>  //파일의 정보를 언디 위한 헤더 파일이다.
#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <stdio.h>
#include <stdlib.h>

int stat(const char *file_name, struct stat *buf);
int fstat(int fd, struct stat *buf);                //fd용 stat함수
int lstat(const char *file_name, struct stat *buf); // symbolic link용 stat함수

//특정 파일에 대한 각종 정보를 담고있는 구조체를 반환
//정보를 조회하는데 파일 자체에 대한 권한은 필요없으나 파일까지 접근하기 까지의 디렉토리에 대해 권한이 필요함
// 버퍼를 통한 디렉토리 정보나, 파일이름, fd로 파일의 포인터를 지정해줄수있음
// lstat()는 Symbolic link자체에 대한 정보를 조회함

/*
struct stat{
    ...
    mode_t st_mode; // 파일 권한을 담는 속성
    ...
    st_blocks // 파일이 차지하고 있는 block의 개수
    st_blksize // IO블록의 개수
    st_atime // 마지막 접근시간
    st_mtime // 마지막 수정시간
    st_ctime // 마지막 상태 변경 시간
}

POSIX시스템에서 File Type 지정
S_ISLNK(st_mode): symbolic link (0170000)
S_ISREG(st_mode): regular file(0140000)
S_ISDIR(st_mode): directory(0120000)
S_ISCHR(st_mode): character device(0100000)
S_ISBLK(st_mode): block device(0060000)
S_ISFIFO(st_mode): named pipe(0040000)
S_ISSOCK(st_mode): socket(0010000)

S_ISUID: set-user-id (0004000)
S_ISGID: set-group-id(0002000)
S_ISVTX: sticky bit(0001000)

*/

int main(int argc, char *argv[])
{
    int i;
    struct stat buf;
    char *ptr;
    for (i = 1; i < argc; i++)
    {
        printf("%s", argv[i]);
        if (lstat(argv[i], &buf) < 0) //심볼릭 링크가 아니라면 걸름
        {
            perror("lstat error");
            continue;
        }
        if (S_ISREG(buf.stmode)) // 순서대로 파일 형식을 비교함
            ptr = "regular";
        else if (S_ISDIR(buf.stmode))
            ptr = "directory";
        else if (S_ISCHR(buf.stmode))
            ptr = "charactor special";
        else if (S_ISBLK(buf.stmode))
            ptr = "block special";
        else if (S_ISFIFO(buf.stmode))
            ptr = "fifo";

#ifdef S_ISLNK //
        else if (S_ISLNK(buf, st_mode))
            ptr = "symbolic link";
#endif
#ifdef S_ISSOCK
        else if (S_ISSOCK(buf, st_mode))
            ptr = "socket";
#endif
        else
            ptr = "** unknown mode **";
        printf("%s\n, ptr");
    }
    exit(0);
}