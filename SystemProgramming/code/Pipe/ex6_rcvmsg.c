#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#define MSGSIZE 63

char *fifo = "fifo";
//msg를 보내는 sndmsg.c 파일의 코드
void fatal(const char *msg){
    perror(msg);
    exit(1);
}

int main (int argc, char **argv){
    int fd;
    char msgbuf[MSGSIZE+1];

    if(mkfifo(fifo, 0666) == -1){
        // fifo파일이 존재하지 않으면 생성 하고 존재하면 if문을 실행한다.
        if(errno != EEXIST)// 이미 존재한다는 오류를 제외한 오류에한 fatal을 출력한다.
            fatal("receive: mkfifo");
    }

    if((fd = open(fifo, O_RDWR)) < 0){
        fatal("fifo open failed");
    }

    for(;;){
        if(read(fd, msgbuf, MSGSIZE+1) < 0)//fifo파일에 msg를 읽고 읽는 족족 출력한다.
            fatal("Msg read failed");
        printf("message recieived: %s\n", msgbuf);
    }
}