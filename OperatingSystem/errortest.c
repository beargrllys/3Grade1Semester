#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    int call_count = 0;
    int status;
    pid_t pid;

    pid = fork();
    call_count += 1;
    if (pid == 0)
    {
        printf("Call by child %d\n", call_count);
        printf("I\'m Child!\n");
    }
    else if (pid > 0)
    {
        printf("Call by parent %d\n", call_count);
        waitpid(-1, &status, 0);
        printf("I\'m Parent!\n");
    }
    else
    {
        perror("FORK ERROR:");
        exit(0);
    }
    return 0;
}