// int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval_ *timeout);
//여러개의 fd를 파이프를 동시에 처리 한다.
//모든 디바이스가 처리 될수 있도록한다.
// nfds는 전체 discriptor의 개수의 +1개로 할당한다.
// 비트 마스크 를 사용하여 Fileset를 설정한다.

//FD_ZERO fdset을 0으로 초기화
//FD_SET(int fd, fd_set fd);// 특정 fd 번호를 1로 변경
//FD_ISSET(int fd, fd_set *fdset); // 특정 fd번호가 fdset에서 1로 되어 있는지 반환
//void FD_CLR(int fd, fd_set *fdset); // 특정 fd번호를 fd set에서 0으로 초기화

//timeout은 ms단위로 시간을 설정할수 있으며 일정시간 멈춘후(sleep) select함수를 실행한다.
//반환값 에러시 -1, 준비된 descriptor가 없으면 0, 이외에는 비트마스크 되어있는 준비된 파일의 개수를 반환한다.
//그래서 함수 내부에서는 준비된 파일의 개수를 바로 알수 없으며 반목분을 통해 체크해야함
//비효율적임

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define MSGSIZE 6


char *msg1 = "hello";
char *msg2 = "bye";
void parent(int [][2]);
int child(int []);

void fatal (const char *msg){
    perror(msg);
    exit(1);
}

int main(){
    int pip[3][2];//file discrptor 3개
    int i;

    for(i = 0; i< 3; i++){
        if(pipe(pip[i]) == -1){//파이프 3개 만들기
            fatal ("pip call");
        }

        switch (fork()){
            case -1:
            fatal("fork call");
            case 0:
            child(pip[i]);//각각 fork를 통해 child만들기

        }
    }
    parent(pip);
    exit(0);
}

void parent(int p[3][2]){
    char buf[MSGSIZE], ch;
    fd_set set, master;
    int i;

    for (i = 0; i< 3; i++)
        close(p[i][1]);// 부모프로세스에 write 부분 닫기
    FD_ZERO(&master);
    FD_SET(0, &master);//stdin을 보겠다.
    for(i=0; i<3; i++)
        FD_SET(p[i][0], &master);

    while(set = master, select (p[2][0]+1, &set, NULL,NULL,NULL) >0){//p[2][0]+1 nfds = MAX+1= fd의 개수
        if(FD_ISSET(0, &set)){
            printf("From standard input...");
            read(0,&ch, 1);
            printf("%c\n", ch);
        }
        for(i=0; i<3; i++){
            if(FD_ISSET(p[i][0], &set))
                if(read(p[i][0], buf, MSGSIZE) > 0){
                    printf("Message form child%d\n",i);
                    printf("MSG=%s\n", buf);
                }
        }
        if(waitpid(-1, NULL,WNOHANG) == -1)
            return;
    }
}

int child(int p[2]){
    int count;
    close(p[0]);
    for(count = 0; count < 2; count++){
        write(p[1], msg1, MSGSIZE);
        sleep(getpid()%4);
    }

    write(p[1], msg2, MSGSIZE);
    exit(0);
}