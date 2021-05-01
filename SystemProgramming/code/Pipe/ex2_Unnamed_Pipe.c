#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <limits.h>

int count;
void alrm_action(int signo)
{
    printf("write blocked after %d characters\n", count);
    exit(0);
}

int main()
{
    int p[2];
    int pipe_size;
    char c = 'X';
    static struct sigaction act;

    act.sa_handler = alrm_action;
    sigfillset(&(act.sa_mask));
    sigaction(SIGALRM, &act, NULL); //signal 준비
    if (pipe(p) < 0)
    { // 파이프 개설
        perror("pipe call");
        exit(1);
    }
    pipe_size = fpathconf(p[0], _PC_PIPE_BUF); //fpathconf(fd,_PC_PIPE_BUF) 파이프에 자동적으로 쓰기최대 바이트수.
    printf("Maximum size if write to pipe: %d bytes\n", pipe_size);
    while (1)
    {
        alarm(20);
        write(p[1], &c, 1);        //파이프에 데이터 퍼나르는중
        alarm(0);                  // 파이프에 read하다가 파이프가 가득차면 최종 용량 전시
        if ((++count % 1024) == 0) //1024단위로 pipe용량 전시
            printf("%d character int pipe\n", count);
    }
}

/*
Maximum size if write to pipe: 4096 bytes
1024 character int pipe
2048 character int pipe
3072 character int pipe
4096 character int pipe
5120 character int pipe
6144 character int pipe
7168 character int pipe
8192 character int pipe
9216 character int pipe
10240 character int pipe
11264 character int pipe
12288 character int pipe
13312 character int pipe
14336 character int pipe
15360 character int pipe
16384 character int pipe
17408 character int pipe
18432 character int pipe
19456 character int pipe
20480 character int pipe
21504 character int pipe
22528 character int pipe
23552 character int pipe
24576 character int pipe
25600 character int pipe
26624 character int pipe
27648 character int pipe
28672 character int pipe
29696 character int pipe
30720 character int pipe
31744 character int pipe
32768 character int pipe
33792 character int pipe
34816 character int pipe
35840 character int pipe
36864 character int pipe
37888 character int pipe
38912 character int pipe
39936 character int pipe
40960 character int pipe
41984 character int pipe
43008 character int pipe
44032 character int pipe
45056 character int pipe
46080 character int pipe
47104 character int pipe
48128 character int pipe
49152 character int pipe
50176 character int pipe
51200 character int pipe
52224 character int pipe
53248 character int pipe
54272 character int pipe
55296 character int pipe
56320 character int pipe
57344 character int pipe
58368 character int pipe
59392 character int pipe
60416 character int pipe
61440 character int pipe
62464 character int pipe
63488 character int pipe
64512 character int pipe
65536 character int pipe
write blocked after 65536 characters
*/