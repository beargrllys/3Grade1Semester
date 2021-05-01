//파이프는 두 프로세스간에 단방향 통신채널이다.
// 일반적으로 부모 자식 관계에서 연경하는 용도로 쓰인다.
//named(관련없는 프로세스 간에 통신)와 unamed pipe(부모 자식 간의 파이프)가 있다
// 2개의 fd(R/W)를 제작한다. 파이프는 , FIFO 파일이다.

//Unamed Pipe
// read하려는데 파이프가 비어있으면 Block, write하려는데 파이프가 가득 차있으면 block
// 모든 writer가 파이프에 데이터를 쓰지 않으면 reader는 곧 EOF에 다다른다. =>writer가 waiting
// 모든 Reader가 파이프의 데이터를 읽지 않으면 writer는 곧 Broken Pipe에 다다른다. =>reader가 waiting

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define MSGSIZE 16;

static char *msg1 = "hello, world #1";
static char *msg2 = "hello, world #2";
static char *msg3 = "hello, world #3";

int main(void)
{
    char buf[MSGSIZE];
    int fd[2], i, pid;

    if (pipe(fd) < 0)
    {
        perror("pipe call");
        exit(1);
    }

    if ((pid = fork()) < 0)
    {
        perror("fork call");
        exit();
    }
    // 자식  >>>> 부모
    //만약 자식프로세서라면 read fd를 닫고 wirte fd를 연다.
    if (pid == 0)
    {
        close(fd[0]);
        write(fd[1], msg1.MSGSIZE);
        write(fd[1], msg2.MSGSIZE);
        write(fd[1], msg3.MSGSIZE);
    }

    //만약 부모프로세서라면 wirte fd를 닫고 read fd를 연다.
    else
    {
        close(fd[1]);
        for (i = 0; i < 3; i++)
        {
            read(fd[0], buf, MSGSIZE);
            printf("%s\n", buf);
        }
        wait(NULL);
    }
    exit(0);
}

// 만약 pipe를 개설하고 fork할때 상속되는 fd를 fork한다면?  parent 와 child 간에 읽기 창구와 쓰기 창구를 교차하여 열고 닫음으로서 pipe가 동작함
