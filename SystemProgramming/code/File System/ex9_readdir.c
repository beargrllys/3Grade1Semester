#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <sys/stat.h>  //파일의 정보를 언디 위한 헤더 파일이다.
#include <unistd.h>    //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <fcntl.h>     // 파일을 다루기 위한 헤더파일이다. file control
#include <dirent.h>    //디렉토리 관련 함수 모음 헤더파일
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mkdir(const char *pathname, mode_t mode);
//path name에 해당하는 디렉토리를 만드는 것을 시도합니다.
//mode에 있는 권한으로 디렉토리 접근권한을 설정합니다.
//성공하면 - 실패하면 -1을 반환합니다.

int rmdir(const char *pathname);
//디렉토리를 삭제합니다. 다만 해당 디렉토리는 모두 비어있어햐합니다.
//성공하면 - 실패하면 -1을 반환합니다.

DIR *opendir(const char *name);
//name에 해당하는 디렉토리를 열러 directory stream을 열고, 해당 스트림의 포인터를 반환합니다.
//에러가 발생하면 NULL를 발생시킴니다.

int closedir(DIR *dir);
//들어온 dirstream을 닫습니다.
//성공하면 - 실패하면 -1을 반환합니다.

struct dirent *readdir(DIR *dir);
//dir이 가리키는 디렉토리 스트림의 다음 디렉토리 항목에 대한 구조체 포인터를 반환합니다.
//readdir에 의해 반환된 데이터는 동일한 directory stream에 대한 후속호출에 의해 덮어씌워집니다.
//일반적으로는 dirent 구조체의 포인터가 반한되고 파일을 모두 읽었거나 에러가 발생했을때 NULL을 반환합니다.

void rewinddir(DIR *dir);
//덮어씌워지던 directory stream의 위치를 다시 디렉토리의 처음으로 돌려 보냅니다.

void fatal(const char *str, int no)
{
    perror(str);
    exit(no);
}

void access_perm(char *perm, mode_t mode)
{
    int i;
    char permchar[] = "rwx";
    memset(perm, '-', 10);
    perm[10] = '\0';
    if (S_ISDIR(mode))
        perm[0] = 'd';
    else if (S_ISCHR(mode))
        perm[0] = 'c';
    else if (S_ISBLK(mode))
        perm[0] = 'b';
    else if (S_ISFIFO(mode))
        perm[0] = 'p';
    else if (S_ISLNK(mode))
        perm[0] = 'l'; //해당 파일의 모드가 어떤 파일인지 구분함

    for (i = 0; i < 9; i++)
        if ((mode << i) & 0x100)           // 0x100 == 0001 0000 0000
            perm[i + 1] = permchar[i % 3]; //mode에 있는 문자를 비트 이동을 이용하여 0,1의 순서대로 권한 소유 여부를 확인한다.
    if (mode & S_ISUID)
        perm[3] = 's'; //set_uid_bit
    if (mode & S_ISGID)
        perm[6] = 's'; //set_gid_bit
    if (mode & S_ISVTX)
        perm[9] = 't'; //특수비트 sticky bit
}

int main(int argc, char *argv[])
{
    DIR *dp;
    struct stat statbuf;
    struct dirent *dent;
    char perm[11];
    char pathname[80];

    if (argc < 2)
        fatal("no directory name is provided", 1);

    if (access(argv[1], F_OK) == -1)
        fatal("access error", 1);
    if (stat(argv[1], &statbuf) < 0)
        fatal("stat error", 1);
    if (!S_ISDIR(statbuf.st_mode))
        fatal("no dir", 1);
    if ((dp = opendir(argv[1])) == NULL)
        fatal("opendir error", 1);

    printf("List of Directory(%s):\n", argv[1]); //출력: List of Directory(foo-d):
    while ((dent = readdir(dp)) != NULL)         //directory steam 정보를 하나씩 가져옴
    {                                            //readdir 로 directory steam을 받아옴
        sprintf(pathname, "%s/%s", argv[1], dent->d_name);
        if (stat(pathname, &statbuf) < 0)
            fatal("stat error", 1);
        access_perm(perm, statbuf.st_mode);
        printf("%s %8ld %s\n", perm, statbuf.st_size, dent->d_name); //출력: -rwxrwxrwx     2457 ex1_SimpleFileIO.c
    }

    closedir(dp);
    exit(0);
}