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
    return count - 1; // return command size
}

int clear_args(char *inputBuf[])
{
    int i = 0;
    while (*(inputBuf + i) != NULL)
    {
        if (inputBuf[i] != NULL)
        {
            free(inputBuf[i]);
        }
    }
    memset(inputBuf, '\0', (MAX_LINE / 2 + 1) * sizeof(char));
    return 0;
}

int print_input(char *inputBuf[], int *size)
{
    int count = 0;
    for (count = 0; count <= *size; count++)
    {
        printf("%s\n", *(inputBuf + count));
    }
    return 0;
}

void fileRedirection(char *inputBuf[], int *token, int *LtoR, int *argc)
{
    int idx = 0;
    int fd;
    char *paramList[MAX_LINE / 2 + 1];
    fd = open(*(inputBuf + *argc), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    printf("%s\n", *(inputBuf + *argc));
    if (fd < 0)
    {
        perror("file open error");
        exit(1);
    }
    if (*LtoR == 1)
    { //Left to Right
        for (int tmp = 0; tmp < idx + 2; tmp++)
        {
            paramList[tmp] = (char *)malloc(sizeof(char) * MAX_LINE);
            if (tmp == idx)
            {
                *(paramList + tmp) = ".";
            }
            else if (tmp == idx + 1)
            {
                *(paramList + tmp) = NULL;
            }
            else
            {
                strcpy(paramList[tmp], inputBuf[tmp]);
            }
        }
    }
    else
    { //Right to Left
        for (idx = *token + 1; idx < *argc; idx++)
        {
            *(paramList + idx) = *(inputBuf + idx);
        }
    }
    printf("%s   %s\n", *paramList, *(paramList + 1));
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("failed open");
        exit(1);
    }
    if (execvp(*paramList, paramList) == -1)
    {
        perror("failed exec");
        exit(1);
    }
    close(fd);
}

int whatCommand(int *argc, char *inputBuf[])
{
    int count = 0;
    int token = 0;
    int LtoR = 0;
    char *temp;
    while (*(inputBuf + count) != NULL)
    {
        temp = *(inputBuf + count);
        if (strcmp(temp, ">") == 0)
        {
            LtoR = 1;
            fileRedirection(inputBuf, &token, &LtoR, argc);
        }
        else if (strcmp(temp, "<") == 0)
        {
            ;
        }
        else if (strcmp(temp, "|") == 0)
        {
            ;
        }
        else if (strcmp(temp, "&") == 0)
        {
            ;
        }
        else
        {
            token++;
            count++;
        }
    }
    return 0;
}

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1;
    int size;
    pid_t pid;

    memset(args, '\0', (MAX_LINE / 2 + 1) * sizeof(char));

    while (should_run)
    {
        printf("osh>");
        size = std_input(args);
        //print_input(args, &size);
        whatCommand(&size, args);
        fflush(stdout);
        /**
    * After reading user input, the steps are:
    * (1) fork a child process using fork()
    * (2) the child process will invoke execvp()
    * (3) parent will invoke wait() unless command included &
    */
    }
    return 0;
}