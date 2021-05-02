//popen:command string을 받아 실행한다. 성공시 file pointer을 받고 실패시 NULL을 받아온다.
//pclose: 파이으를 닫는다. 성공시 termination status를 실패시 -1을 반환한다.
// 앞에서 했던 파이프를 다루기 위한 자질구래한 과정들을 한번에 수행해준다.

//본질적으로 popen은 fork를 해서 매개변수로 받은 cmdstring을 exec를 해주는 함수이다.
//type의 경우에는 r일 경우 자식이 보낸 메세지를 부모입장에서 읽는 것이고 w의 경우 부모입장에서 자식에게 쓰기 해주는 변수이다. 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PAGER "/bin/more"
#define MAXLINE 1024

int main(int argc, char *argv[]){
    char line[MAXLINE];
    FILE *fpin, *fpout;
    if(argc != 2)
        perror("usage: a.out <pathname>");//[프로그램이름] [pathname]
    if((fpin = fopen(argv[1], "r")) == NULL)//부모가 pathname을 열어서 그 안에 있는 내용을 읽겠다
        fprintf(stderr, "can't open $s, argv[1]");
    if((fpout = popen(PAGER, "w") == NULL))// 한개의 함수가 [파이프 열기] => [fork하기] => [안쓰는 fd 닫기] => [exec 수행하기] =>[wait하기]의 일련의 과정을 bin/more함수를 수행하기 위해 수행.  부모가 자식에게 write하겠다. 읽고 PAGER 명령어를 exec하겠다.
        perror("popen error");

    //여기서 부터는 부모프로세스가 값을 출력하면 more가 exec된 형태로 수행하게 된다.

    while(fgets(line, MAXLINE, fpin) != NULL){
        if(fputs(line, fpout) == EOF){
            perror("fputs error to pipe");
        }//값을 쭉 출력해준다
    }
    if(ferror(fpin))
        perror("fgets error");
    if(pclose(fpout) == -1)
        perror("pclose error");

    exit(0);
}