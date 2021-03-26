#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define MAX_LINE 80 /* The maximum length command */
#define FPERM 0644
#define BUFF_SIZE 1024
#define READ_END 0
#define WRITE_END 1

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
    while ((i < (MAX_LINE / 2 + 1)))
    {
        //free(inputBuf[i]);
        inputBuf[i] = NULL;
        i++;
    }
}

int print_input(char *inputBuf[], int *size)
{
    int count = 0;
    for (count = 0; count <= *size; count++)
    {
        printf("%s\n", *(inputBuf + count));
    }
}

void fileRedirection(char *inputBuf[], int *token, int *LtoR, int *argc)
{
    int idx = 0;
    int fd, ret;
    char *paramList[MAX_LINE / 2 + 1];
    fd = open(*(inputBuf + *argc), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0)
    {
        perror("file open error");
        exit(1);
    } //Left to Right

    for (idx = 1; idx < *token; idx++)
    {
        *(paramList + idx - 1) = *(inputBuf + idx);
    }
    for (int tmp = 0; tmp < idx + 1; tmp++)
    {
        paramList[tmp] = (char *)malloc(sizeof(char) * MAX_LINE);
        if (tmp == idx)
        {
            //*(paramList + tmp) = ".";
            *(paramList + tmp) = NULL;
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
    ret = dup2(STDOUT_FILENO, fd);
    //clear_args(paramList);
}

void orderRedirection(char *inputBuf[], int *token, int *argc)
{
    char *content = (char *)malloc(sizeof(char) * MAX_LINE);
    char *paramList[MAX_LINE / 2 + 1];
    char buf[BUFF_SIZE];
    int idx = *token;
    int fd;
    ssize_t rd_size;

    memset(content, '\0', MAX_LINE);

    if ((fd = open(*(inputBuf + *argc), O_RDONLY)) > 0)
    {
        while (0 < (rd_size = read(fd, buf, BUFF_SIZE)))
        {
            strcat(content, buf);
            buf[rd_size] = 0;
        }
    }
    else
    {
        perror("file open error");
        exit(1);
    }

    close(fd);
    for (int tmp = 0; tmp < idx + 2; tmp++)
    {
        paramList[tmp] = (char *)malloc(sizeof(char) * MAX_LINE);
        if (tmp == idx)
        {
            strcpy(paramList[tmp], buf);
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
    if (execvp(inputBuf[0], paramList) == -1)
    {
        perror("failed exec");
        exit(1);
    }
}

int seperateOrder(char *inputBuf[], char *front_order[], char *back_order[], int *tocken, int *argc)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < *tocken; i++)
    {
        front_order[i] = (char *)malloc(sizeof(char) * MAX_LINE);
        memset(front_order[i], '\0', MAX_LINE);
        strcpy(front_order[i], inputBuf[i]);
    }
    front_order[*tocken] = NULL;
    for (i = *tocken + 1; i <= *argc; i++)
    {
        back_order[j] = (char *)malloc(sizeof(char) * MAX_LINE);
        memset(back_order[j], '\0', MAX_LINE);
        strcpy(back_order[j], inputBuf[i]);
        //printf("%s  ==   %d   \n", back_order[j],  *tocken+1 + i);
        j++;
    }
    return *argc - *tocken + 1;
}

int pipeline(char *inputBuf[], int *token, int *argc)
{
    char *frontParam[MAX_LINE / 2 + 1];
    char *behindParam[MAX_LINE / 2 + 1];
    char buffer[BUFF_SIZE];
    int fd[2];
    int status = 2;
    int order2_size;
    pid_t pid;
    FILE *fp = NULL;
    size_t read_size = 0;

    order2_size = seperateOrder(inputBuf, frontParam, behindParam, token, argc);

    if (pipe(fd) == -1)
    {
        printf("PIPE ERROR\n");
        fprintf(stderr, "Pipe failed");
        return -1;
    }

    //READ_END 0
    //WRITE_END 1
    pid = fork();
    if (pid < 0)
    {
        printf("FORK ERROR\n");
        fprintf(stderr, "Fork failed");
        return 1;
    }

    else if (pid > 0)
    {
        close(fd[READ_END]);
        if (dup2(fd[WRITE_END], STDOUT_FILENO) == -1)
        {
            perror("failed open");
            exit(1);
        }
        if (execvp(*frontParam, frontParam) == -1)
        {
            perror("failed PARENT exec");
            exit(1);
        }
        //PARENT PROCESS
        close(fd[WRITE_END]);
        waitpid(-1, &status, 0);
    }
    else
    {
        //CHILD PROCESS
        close(fd[WRITE_END]);
        if (dup2(fd[READ_END], STDIN_FILENO) == -1)
        {
            perror("failed open");
            exit(1);
        }
        close(fd[READ_END]);
        printf("%s  |   %s   \n", behindParam[0], behindParam[1]);

        if (execvp(*behindParam, behindParam) == -1)
        {
            perror("failed CHILD exec");
            exit(0);
        }
        return 0;
    }
    return 0;
}

void normal_exec(char *inputBuf[])
{

    if (execvp(inputBuf[0], inputBuf) == -1)
    {
        perror("failed CHILD exec");
        exit(0);
    }
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
            LtoR = 0;
            orderRedirection(inputBuf, &token, argc);
        }
        else if (strcmp(temp, "|") == 0)
        {
            pipeline(inputBuf, &token, argc);
        }
        else
        {
            token++;
            count++;
        }
    }
    normal_exec(inputBuf);
    return 0;
}

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1;
    int size, exp;
    int status;
    pid_t pid;

    memset(args, '\0', (MAX_LINE / 2 + 1) * sizeof(char));

    while (should_run)
    {
        printf("osh>");
        size = std_input(args);
        //print_input(args, &size);
        pid = fork();

        if (pid == 0)
        {
            if (strcmp(args[size], "&") != 0)
            {
                whatCommand(&size, args);
            }
            else
            {
                args[size] = NULL;
                exp = size - 1;
                whatCommand(&exp, args);
            }
            return 0;
        }
        else if (pid > 0)
        {
            if (strcmp(args[size], "&") != 0)
            {
                waitpid(-1, &status, 0);
            }
            else
            {
                waitpid(-1, &status, WNOHANG);
            }
        }
        else
        {
            perror("FORK ERROR :");
            exit(0);
        }
        fflush(stdout);
    }
    return 0;
}