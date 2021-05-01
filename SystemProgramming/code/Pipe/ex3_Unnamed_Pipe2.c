#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define MSGSIZE 6

int parent(int *);
int child(int *);

char *msg1 = "hello";
char *msg2 = "bye!!";

void fatal(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    int pid[2];

    if (pipe(pid) < 0)
        fatal("pipe call");

    if (fcntl(pid[0], F_SETFL, O_NONBLOCK) == -1)
        fatal("fcntl call");

    switch (fork())
    {
    case -1:
        fatal("fork call");
    case 0:
        child(pid);
    default:
        parent(pid);
    }
}

int parent(int p[2])
{
    int nread;
    char buf[MSGSIZE];
    close(p[1]);
    for (;;)
    {
        switch (nread = read(p[0], buf, MSGSIZE)) //1초에 한번씩 파이프에 있는 값을 받아와줌
        {
        case -1:
            if (errno == EAGAIN)
            {
                printf("(pipe empty)\n");
                sleep(1); //1초에 한번씩 (pipe empty)
                break;
            }
            else
                fatal("read call");
        case 0: //파이프가 비어있으면 End of conversation 출력
            printf("End of conversation\n");
            exit(0);
        default:
            printf("MSG=%s\n", buf);
        }
    }
}

int child(int p[2])
{
    int count;
    close(p[0]);
    for (count = 0; count < 3; count++)
    {
        write(p[1], msg1, MSGSIZE); //3초에 한번씩 파이프에 hello
        sleep(3);
    }

    write(p[1], msg2, MSGSIZE); //3번 출력하면 bye!! 출력
    exit(0);
}