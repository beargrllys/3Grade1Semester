//Real user ID는 실제로 프로그램을 구동시키는 user이다
//Effective user ID(euid)는 해당 프로그램에 접근가능하고 실행가능한 user의 모음이다.
//일반적으로는 둘이 동일하다
//그래서 euid를 바꾸고 싶으면 setuid() System call을 호출하면 된다.
// 이들은 fork되더라도 exec하더라도 상속되고 유지된다.
// Gruop ID 또한 동일한 관계를 가진다.

#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdlib.h>
#include <stdio.h>

#define CMDLEN 80

uid_t getuid(void);
//현재 프로세스의 Real User ID를 반환합니다.
uid_t geteuid(void);
//현재 프로세스의 Effective User ID를 반환합니다.
gid_t getgid(void);
//현재 프로세스의 Real Group ID를 반환합니다.
gid_t getegid(void);
//현재 프로세스의 Effective Group ID를 반환합니다.

int setuid(uid_t parmuid);
//super user는 Real User ID와 Effective User ID 모두 parmuid로 변경가능하며
//normal user는 자신의 uid가 Real User ID일때만 Effective User ID를 parmuid로 변경 가능하다.
int setgid(gid_t gid);

int chroot(const char *path);
// 현재 프로세스의 root directory를 변경합니다.
// path는 바꿀 디렉토리를 말합니다.
//성공하면 - 실패하면 -1을 반환합니다.

//getpriority() 혹은 setpriority()로 프로세스의 우선순위를 변경합니다.
//프로세스의 nice value을 변경하여 프로세스의 우선순위를 조정하며 리눅스 기준 -20~+19의 값을 설정할 수 있습니다.
//이중 오짓 super user 만이 0미만의 우선 순위를 설정할수 있습니다.

int system(const char *string);
// 특정 명령어를 명령창에 실행합니다. /bin/sh -c를 통해 실행하는 것 이며 해당 명령어의 결과가 전시됩니다.
//fork, exec, waitpid를 통해 구현되었습니다.
// 위 구성 명령어 실행중 오류 발생시 system() 함수 자체에서는 -1을 반환합니다.
//exec에서 실패한 경우 종료 status는 127을 반환합니다.
// 한편 결과 전시 이후에는 waitpid의 형태를 띕니다.

int main(void)
{
    char cmdstr[CMDLEN];
    printf("Enter command to run: ");
    fflush(stdout);
    fgets(cmdstr, CMDLEN, stdin);
    system(cmdstr);
    return 0;
}