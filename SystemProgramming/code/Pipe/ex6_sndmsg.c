//unname 파이프의 단점은 오직 공통된 조상인 경우에만 사용가능하고 영원히 존재하지 않고 다쓰면 사라진다.
//명령어로 named pipe만들기 : mkfifo channel
//FIFO파일은 filetype이 p임

//mkfifo(const char *pathname, mode_t mode)
//fifo 파일  다루는 법

// mkfifo("tmp/fifo", 0666); => fifo 타잎의 파일 만들기
// fd = open("tmp/fifo" ,O_WRONLY); => fifo 파일은 반드시 open 함수로 만들어야함
//이때 해당 fifo 파일에 쓰기를 했는에 읽을 애가 없으면 해당 pipe는 block됨
// 만약 block되는 것을 원치 않으면 fd = open("tmp/fifo", O_WRONLY|O_NONBLOCK); 라고 작성하여 옵션을 줄수 있음 

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#define MSGSIZE 63

char *fifo = "fifo";
//msg를 보내는 sndmsg.c 파일의 코드
void fatal(const char *msg){
    perror(msg);
    exit(1);
}

int main (int argc, char **argv){
    int fd, j, nwrite;
    char msgbuf[MSGSIZE+1];

    if(argc < 2){
        fprintf(stderr, "Usage: sendmessage msg .. \n");
        exit(1);
    }

    if((fd = open(fifo, O_WRONLY | O_NONBLOCK)) < 0)
        fatal("fifo open failed");

    for(j  = 1; j <argc; j++){
        if(strlen(argv[j] > MSGSIZE)){//argument의 길이가 msg size보다 크다면 에러출력
            fprintf(stderr, "message too long %s\n", argv[j]);
            continue;
        }
        strcpy(msgbuf, argv[j]);//작으면 집어 넣음

        if((nwrite = write(fd, msgbuf, MSGSIZE+1)) == -1)
            fatal("message write failed");
    }
    exit(0);
}