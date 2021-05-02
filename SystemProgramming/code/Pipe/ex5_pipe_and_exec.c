//exec를하게 되면 열려있는 fd를 상속 받는다. == 파이프가 그 fd임

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
int join(char *com1[], char *com2[]);
void fatal(const char *msg){
    perror(msg);
    exit(1);
}

int main(){
    char *one[4] = {"ls", "-l", "/usr/lib", NULL};
    char *two[3] = {"grep", "^d", NULL};//^부호의 의미는 caret이며 ^다음에 나오는 문자가 제일 앞에 위치한  문장만 가져옴
    //그래서 ls의 stdout에서 d로 시작하는 문장만 grep하여 출력하는데 ls의 경우 첫문자가 d라서 디렉토리를 말함
    int ret;
     ret = join(one, two);
     printf("join returned %d\n", ret);
     exit(0);
}

int join(char *com1[], char *com2[]){
    int p[2], status;

    switch(fork()){//일단 fork
        case -1:
        fatal("1st fork call in join");
        case 0:
        break;//정상처리
        default:
        wait(&status);
        return (status);
    }

    if(pipe(p) == -1){
        fatal("pipe call in join");
    }

    switch(fork()){
        case -1:
            fatal("2nd fork call in join");
        case 0://쓰는 프로세스
            dup2(p[1],1); //파이프 1번에 1stdout을 복사한다.
            close (p[0]);//read닫기
            close (p[1]); //기존 write닫기 오직 stdout만 열어둠
            execvp(com1[0], com1);
            fatal(" 1st error\n");
        default:
            dup2(p[0], 0); //파이프의 read파트에 0번 stdin을 복사한다.
            close (p[0]);//read닫기
            close (p[1]); //기존 write닫기 오직 stdout만 열어둠
            execvp(com2[0], com2);
            fatal(" 2nd error\n");
    }


}
