#include <unistd.h> //POSIX시스템에서 각종 system call을 정의 해둔 헤더파일
#include <stdio.h>

long int pathconf(const char *path, int name);
long int fpathconf(int fd, int name);
//특정 시스템이나 파일 시스템의 한계를 돌려 받습니다.
//요구되어지는 변수의 이름을 두번째 파리미터에  돌려받습니다.
//성공하면 요구 시스템 사항을 돌려받고, 실패시 -1을 반환

typedef struct
{
    int val;
    char *name;
} Table;

int main()
{
    Table *tb;
    static Table option[] = {
        {_PC_LINK_MAX, "Maximum number if links"},
        {_PC_NAME_MAX, "Maximum length of a filename"},
        {_PC_PATH_MAX, "Maximum length of a pathname"},
        {-1, NULL}};

    for (tb = option; tb->name != NULL; tb++)
    {
        printf("%-28.28s\t%ld\n", tb->name, pathconf("./foo-d", tb->val));
    }

    printf("Clock Tick : %ld\n", sysconf(_SC_CLK_TCK));
    printf("MAX Open File : %ld\n", sysconf(_SC_OPEN_MAX));
    printf("Max Login Name Length : %ld\n", sysconf(_SC_LOGIN_NAME_MAX));
}