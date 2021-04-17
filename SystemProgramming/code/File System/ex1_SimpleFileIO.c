#include <sys/stat.h>  //파일의 정보를 언디 위한 헤더 파일이다.
#include <fcntl.h>     // 파일을 다루기 위한 헤더파일이다. file control
#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일

#define BSIZE 1024
#define FPERM 0644

int open(const char *pathname, int flags, [mode_t mode]);
//파일을 열람을 시도하고 fd를 반환한다.
// mode는 새로운 파일이 생성될때에만 사용되며 그때 파일의 권한을 설정해준다.
//O_EXCL: 이미 파일이 존재 할때 오류를 발생한다.
//O_TRUNC: 이미 파일이 존재할때 존재하는 파일내용을 지우고 새로 쓴다.
//O_APPEND: 파일을 이어 붙힐수 있도록 모드를 열람힙니다.
//정상적으로 작업을 종료하면 fd를 반환한다.
//열람시 권한이 없으면 에러가 발생한다.

int creat(const char *pathname, mode_t mode);
//단순히 파일을 생성한다.
//O_CREATE|O_WRONLY|O_TRUNC flag를 가지면 open()함수랑 동일하다.

int close(int fd);
//fd를 닫는다.
//프로세스가 종료될때 모든 열렸던 파일을 전부 종료합니다.
//성공시 0 실패시 -1

ssize_t read(int fd, void *buf, size_t count);
//count 사이즈 만큼 fd의 데이터를 읽어서 buf 버퍼에 저장합니다.
//만일 읽은 사이즈가 즉, count가 0이면 0을 반환하고 다른 일을 하지 않습니다.
//에러가 발생하면 -1을 반환합니다.

ssize_t write(int fd, const void *buf, size_t count);
//buf버퍼에 있는 데이터를 count 크기만큼 fd에 데이터를 씁니다.
//count가 0이면 아무것도 쓰지 않습니다.
// 에러 발생시 -1을 반환합니다.

int main(int srgc, char *argv[])
{
    int fd1, fd2, n;
    char buf[BSIZE];

    if (argc < 3)
    {
        fprintf(stderr, "Usage; %s arc dest\n", argv[0]);
        exit(1)
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        perror("file open error");
        exit(1);
    }
    if ((fd2 = create(argv[2], FPERM)) < 0)
    { // 파일을 쓸수있는 권한으로 생성
        perror("file creation error");
        exit(1);
    }

    while ((n = read(fd1, buf, BSIZE)) > 0)
    {
        write(fd2, buf, n);
    }

    close(fd1);
    close(fd2);
}