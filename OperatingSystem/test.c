#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define MAX_LINE 80 /* The maximum length command */
#define FPERM 0644

int std_input(char *inputBuf[]);
int clear_args(char *inputBuf[]);
int command_exe(char *order);

int std_input(char *inputBuf[]) //Shell input function
{
    int count = 0;
    char temp_str[MAX_LINE];
    char *ptr;

    clear_args(inputBuf);
    scanf("%[^\n]s", temp_str);
    getchar();
    ptr = strtok(temp_str, " ");
    while (ptr != NULL)
    {
        inputBuf[count] = (char *)malloc(sizeof(char) * MAX_LINE);
        strcpy(inputBuf[count], ptr);
        ptr = strtok(NULL, " ");
        count++;
    }
    return count; // return command size
}

int clear_args(char *inputBuf[])
{
    int i = 0;
    while (*(inputBuf + i) != NULL)
    {
        memset(inputBuf, '\0', (MAX_LINE / 2 + 1) * sizeof(char));
    }
}

int print_input(char *inputBuf[], int *size)
{
    int count = 0;
    for (count = 0; count < *size; count++)
    {
        printf("%s\n", *(inputBuf + count));
    }
}

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1;
    int size;
    pid_t pid;
    while (should_run)
    {
        printf("osh>");
        size = std_input(args);
        print_input(args, &size);
        //fflush(stdout);
        /**
    * After reading user input, the steps are:
    * (1) fork a child process using fork()
    * (2) the child process will invoke execvp()
    * (3) parent will invoke wait() unless command included &
    */
    }
    return 0;
}