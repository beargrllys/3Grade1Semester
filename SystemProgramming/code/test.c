#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

static void charactime(char *str)
{
    char *ptr;
    int c;
    setbuf(stdout, NULL);

    for (ptr = str; c = *ptr++;)
    {
        putc(c, stdout);
    }
}

void sig_handler(int signo)
{
    ;
}

int main(void)
{
    sigset_t set, oset;

    pid_t pid;
    if ((pid = fork()) < 0)
    {
        perror("fork error\n");
        exit(1);
    }
    else if (pid == 0)
    {
        //signal(SIGALRM, (void *)sig_handler);
        sigaddset(&set, SIGABORT);
        sigprocmask(SIG_BLOCK, &set, &oset);
        printf("memebr: %d\n", sigismember(&set, SIGABRT));
        printf("ABROT??\n");
        pause();
        charactime("output Child\n");
    }
    else
    {
        charactime("output Parent\n");
        kill(pid, SIGABRT);
    }
    exit(0);
}