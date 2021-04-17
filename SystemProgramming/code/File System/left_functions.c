#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>

int chown(const char *path, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
int lchown(const char *path, uid_t owner, gid_t group);
//path 나 fd를 통해 파일의 owner를 바꿔줍니다.
//super user과 기존 파일주인만이 주인장을 바꾸어 줄수 있습니다.
//파일의 주인은 주인이 속해 있는 어떠한 그룹의 맴버에게라도 주인 권한을 바꿔줄수 있습니다.
//super user가 아닌 사용자가 실행 파일의 소유자나 그룹을 변경하게 되면 S_ISUID, S_ISGID 모드 비트를 지워버립니다.
//성공하면 0  실패하면 -1을 반환합니다.
//ichown은 심볼릭 링크 파일에서 심볼릭링크 파일 자체의 파일 소유자를 변경하는 함수입니다.

int turncate(const char *path, off_t length);
int ftruncate(int fd, off_t length);
//파일의 크기를 length크기 만큼 바꾸어 줍니다.
//만약 기존의 데이터가 length보다 크다면 넘치는 부분은 잘립니다.
//만약 기존의 데이터가 length보다 작다면 남는 부분은 \0f로 채워집니다.
//성공하면 - 실패하면 -1을 반환합니다.

int link(const char *oldpath, const char *newpath);
//파일을 가르키는 새로운 링크파일을 만듭니다.(hard link)
//만약 newpath가 이미 존재한다면 덮더씌워지지 않습니다.
//새로운 링크 이름 이 생긴하면 거기서 하는 작업은 전적으로 이전의 이름과 동일하게 적용됩니다.
// 기존의 이름이 무엇이었는지 구분할수 없습니다.
//오직 super user만이 디렉토리에 대한 hard link를 개설할수 있습니다.

int unlink(const char *pathname);
//파일과 링크된 이름들을 삭제합니다.
//마지막으로 남아 있는 링크가 삭제 시도 될때 해당 파일이 열려있지 않으면 해당 파일이 삭제 됩니다.
// 만일 위 상황에서 파일이 열람된 상태 였다면 해당파일이 close될 떄까지 존재하다가 삭제 됩니다.
//만일 해당이름이 hard link가 아닌 symbolic link라면 해당 링크만 삭제 됩니다.
//성공하면 - 실패하면 -1을 반환합니다.

int rename(const char *oldpathm const char *newpath);
//이름을 oldpath에서 newpath로 바꾸어 줍니다.
//만약 oldpath가 존재하지 않거나 newpath가 이미 존재한다면 프로그래머에게 알립니다. implementation-dependent
//성공하면 - 실패하면 -1을 반환합니다.

int symlink(const char *oldpath, const char *newpath);
//oldpath의 링크를 포함한 새로운 심볼릭링크를 생성합니다.
//심볼릭 링크는 실행중 해석됩니다.
//Dangling link: 존재하지 않는 파일을 가리키는 링크
//링크에 연결되는 소유 권한은 반영되지 않습니다.
//링크 제거시도, 이름변경, 스티커비트가 존재하는 디렉토리에 있을 때 사용권한을 확인합니다.
//newpath가 이미 존재하면 덮어 씌워지지 않습니다.
//성공하면 - 실패하면 -1을 반환합니다.

int readlink(const char *path, char *buf, size_t bufsize);
//symbolic link의 값을 따라가지 않고 읽어 옵니다.
//symbolic link내에는 path의 값이 저장된 buf와 buf 사이즈가 저장되어있습니다.
