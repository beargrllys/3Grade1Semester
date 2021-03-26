#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Open 함수
    int open (const char *pathname, int flag, [mode_t mode]);
    파일을 열고 File descriptor를 반환한다.
    mode 속성은 새로운 파일을 열때에만 사용된다.

    FLAGS
    O_RDONLY, O_WRONLY, O_RDWR
    O_CREAT : 파일이 없을 경우 생성한다.
    O_EXCL: O_CREAT에 의해 파일이 생성될 때 파일이 존재할 경우 에러를 발생한다.
    O_TRUNC: 파일이 이미 정해진 경우 버퍼를 덮어 씌운다.
    O_APPEND: 처음 파일을 쓸때 파일 포인터를 마지막에 씌운다.  

    modes
    O_CREAT 플래그가 있을 때만 사용되며 새로 만들어진 파일의 권한을 설정한다. 다른 경우엔 사용되지 않는다.

    반환값
    새로운 File discriptor를 반환하며 -1을 반환하면 오류가 발생한 것이다.
*/
int fd;
fd = open("/home/teach", O_WRONLY | O_TRUNC);
if (fd = -1) /* error */

    /* Creat 함수
    int open (const char *pathname, mode_t mode);
    새로운 파일을 생성한다. 이 함수는 다음과 같다.

    int open (const char *pathname, mode_t mode){
        return open(name, O_WRONLY | O_CREAT | O_TURNC, mode);
    }

    반환값
    open(name, O_WRONLY | O_CREAT | O_TURNC, mode)과 동일한 성질을 가진다.
*/

    int fd;
