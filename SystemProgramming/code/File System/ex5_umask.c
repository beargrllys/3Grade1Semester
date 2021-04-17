#include <sys/types.h> // 헤더파일에서 특별히 쓰이는 여러 데이터 유형을 모아둔 헤더파일이다.
#include <sys/stat.h>  //파일의 정보를 언디 위한 헤더 파일이다.
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // 파일을 다루기 위한 헤더파일이다. file control

mode_t umask(mode_t mask);
//file mode에 필요한 마스크에 mask를 덮어주고 기존의 값을 반환해줍니다.
//파일을 생성할때 mask될 비트가 on되어 있으면 mask를 위해 꺼준다.
//System call에 해당함
// 무조건 mask 하여 성공시킴 mask해주면 설정 이전의 mode를 반환해줌
//umask는 shell command이기도 함

int main(void)
{

    umask(0); //아무것도 mask시키지 않음

    if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | s_IWOTH) < 0)
    {
        perror("creat error for foo");
        exit(1);
    }

    /*
    출력
    -rw-rw-rw- 

    정상적으로 권한이 설정됨
    */

    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); //----rw-rw-에 해당하는 권한을 mask함

    if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | s_IWOTH) < 0)
    {
        perror("creat error for foo");
        exit(1);
    }

    /*
    출력
    -rw-------

    mask된 값은 바뀌지 않고 해당사항이 없는 권한 수정만 이루어짐
    */

    exit(0);
}