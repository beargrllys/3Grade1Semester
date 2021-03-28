#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define MAX_LINE 80
#define FPERM 0644
#define BUFF_SIZE 1024
#define READ_END 0
#define WRITE_END 1

/*-------------------------------
Department of Interdisciplinary Computing Technology
2017013072 Yun Ji Sang

Operating System 2021-1 / Project 1 Source Code
2021.03.28
----------------------------------*/

int std_input(char *inputBuf[]);                                                                      // Input and Tokenize command
int clear_args(char *inputBuf[]);                                                                     // reset all input Buffer & local Buffer
int print_input(char *inputBuf[], int *size);                                                         // Print tokenized input data
void fileRedirection(char *inputBuf[], int *token, int *LtoR, int *argc);                             // function for FileRedirection
void orderRedirection(char *inputBuf[], int *token, int *argc);                                       // function for File Content Redirection
int pipeline(char *inputBuf[], int *token, int *argc);                                                // fuction for Pipeline Operation
int seperateOrder(char *inputBuf[], char *front_order[], char *back_order[], int *tocken, int *argc); // Seperate order (Command1 | Command2)
void normal_exec(char *inputBuf[]);                                                                   //Excute normal order
int whatCommand(int *argc, char *inputBuf[]);                                                         //classified User order

int std_input(char *inputBuf[])
{
    int count = 0;
    char temp_str[MAX_LINE]; // save raw text line
    char *ptr;               // temporary save command piece

    clear_args(inputBuf);
    memset(temp_str, '\0', MAX_LINE);

    while (temp_str[0] == '\0')
    {
        printf("osh>");
        scanf("%[^\n]s", temp_str);
        getchar(); //absorb Enter buffer
    }
    ptr = strtok(temp_str, " ");
    while (ptr != NULL) // tokenize and save to input buffer
    {
        inputBuf[count] = (char *)malloc(sizeof(char) * MAX_LINE);
        strcpy(inputBuf[count], ptr);
        ptr = strtok(NULL, " ");
        count++;
    }
    return count - 1; // return command block size
}

int whatCommand(int *argc, char *inputBuf[])
{
    int count = 0;
    int token = 0; // The position of Special letter '|', '<>', exit
    int LtoR = 0;
    char *temp;
    while (*(inputBuf + count) != NULL) //Tour all command And execute appropriate funtion
    {
        temp = *(inputBuf + count);
        if (strcmp(temp, ">") == 0) //file Redirection
        {
            LtoR = 1;
            fileRedirection(inputBuf, &token, &LtoR, argc);
        }
        else if (strcmp(temp, "<") == 0) //order Redirection
        {
            LtoR = 0;
            orderRedirection(inputBuf, &token, argc);
        }
        else if (strcmp(temp, "|") == 0) //piping mission
        {
            pipeline(inputBuf, &token, argc);
        }
        else if (strcmp(temp, "exit") == 0) //kill them all
        {
            kill(getppid(), SIGINT); // send signal to kill parent process
        }
        else
        {
            token++;
            count++;
        }
    }
    normal_exec(inputBuf); // If there is no special letter, exe by normal command fuction
    return 0;
}

int clear_args(char *inputBuf[])
{
    int i = 0;
    while ((i < (MAX_LINE / 2 + 1)))
    {
        if (inputBuf[i] != NULL)
        {
            free(inputBuf[i]);  //free all allocated buffer from string buffer pointer array
            inputBuf[i] = NULL; // and reset all allocated buffer address
        }
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

void normal_exec(char *inputBuf[])
{

    if (execvp(inputBuf[0], inputBuf) == -1) // just execute normal command
    {
        perror("failed CHILD exec");
        exit(0);
    }
}

void fileRedirection(char *inputBuf[], int *token, int *LtoR, int *argc)
{
    int idx = 0;
    int fd, ret;
    char *paramList[MAX_LINE / 2 + 1];
    fd = open(*(inputBuf + *argc), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); //read last File name for redirection
    if (fd < 0)
    {
        perror("file open error");
        exit(1);
    }

    for (idx = 1; idx < *token; idx++) // reset buffer before token
    {
        *(paramList + idx - 1) = *(inputBuf + idx);
    }
    for (int tmp = 0; tmp < idx + 1; tmp++) //copy all parameters buffer until in front of token to local buffer,
    {
        paramList[tmp] = (char *)malloc(sizeof(char) * MAX_LINE);
        if (tmp == idx)
        {
            *(paramList + tmp) = NULL;
        }
        else if (tmp == idx + 1) // make end point last of paeameter list
        {
            *(paramList + tmp) = NULL;
        }
        else
        {
            strcpy(paramList[tmp], inputBuf[tmp]);
        }
    }
    if (dup2(fd, STDOUT_FILENO) == -1) // change target STDOUT to file discriptor
    {
        perror("failed open");
        exit(1);
    }

    if (execvp(*paramList, paramList) == -1) // Excute all command
    {
        perror("failed exec");
        exit(1);
    }
    close(fd);
    ret = dup2(STDOUT_FILENO, fd);
    clear_args(paramList);
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

    if ((fd = open(*(inputBuf + *argc), O_RDONLY)) > 0) // read the file contents
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
    for (int tmp = 0; tmp < idx + 2; tmp++) //arrangement all parameter include file readed content
    {
        paramList[tmp] = (char *)malloc(sizeof(char) * MAX_LINE);
        if (tmp == idx) // put the last parameter that read in file content
        {
            strcpy(paramList[tmp], buf);
        }

        else if (tmp == idx + 1)
        {
            *(paramList + tmp) = NULL;
        }
        else
        {
            strcpy(paramList[tmp], inputBuf[tmp]); //last buffer allocate from input buffer
        }
    }
    if (execvp(inputBuf[0], paramList) == -1) //all arragement data executed
    {
        perror("failed exec");
        exit(1);
    }
    clear_args(paramList);
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

    order2_size = seperateOrder(inputBuf, frontParam, behindParam, token, argc); // serperate two command between token "|"

    if (pipe(fd) == -1) //make the pipe
    {
        printf("PIPE ERROR\n");
        fprintf(stderr, "Pipe failed");
        return -1;
    }

    //READ_END 0
    //WRITE_END 1

    pid = fork(); //start child process

    if (pid < 0)
    {
        printf("FORK ERROR\n");
        fprintf(stderr, "Fork failed");
        return 1;
    }
    else if (pid == 0) // Child process Code
    {
        close(fd[READ_END]);                          //open WRITE_END
        if (dup2(fd[WRITE_END], STDOUT_FILENO) == -1) //change file discriptor target to Pipe
        {
            perror("failed open");
            exit(1);
        }
        if (execvp(*frontParam, frontParam) == -1) //write & execute front command output for parent process
        {
            perror("failed PARENT exec");
            exit(1);
        }
        close(fd[WRITE_END]); // writing Done
        //CHILD PROCESS
        exit(3);
    }
    else // Parent process Code
    {
        waitpid(-1, &status, 0); //waiting child process until front command process Done
        printf(" ");
        close(fd[WRITE_END]);                       //open READ_END
        if (dup2(fd[READ_END], STDIN_FILENO) == -1) //push readed pipe data to STDIN_FILENO
        {
            perror("failed open");
            exit(1);
        }
        close(fd[READ_END]); //close READ_END

        if (execvp(*behindParam, behindParam) == -1) //Excute behind command output and output display
        {
            perror("failed CHILD exec");
            exit(0);
        }
    }
    clear_args(frontParam);
    clear_args(behindParam);
    return 0;
}

int seperateOrder(char *inputBuf[], char *front_order[], char *back_order[], int *tocken, int *argc)
{ // serperate two command between token "|"
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

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1;
    int size, exp;
    int status;
    pid_t pid;

    for (int i = 0; i < (MAX_LINE / 2 + 1); i++)
    {
        args[i] = NULL;
    }

    while (should_run)
    {
        size = std_input(args);
        //print_input(args, &size);
        pid = fork(); // Execute by child process

        if (pid == 0)
        {
            pid = getpid();
            if (strcmp(args[size], "&") != 0) //distinguish the last letter is "&"
            {
                whatCommand(&size, args);
            }
            else
            {
                printf("\nChild [%d]\n", pid); //notice background process is started
                args[size] = NULL;             // excute without last letter "&"
                exp = size - 1;
                whatCommand(&exp, args);
            }
            return 0;
        }
        else if (pid > 0)
        {
            if (strcmp(args[size], "&") != 0)
            {
                waitpid(pid, &status, 0); // parent process waiting for child process
            }
            else
            {
                waitpid(-1, &status, WNOHANG); // parent process dose not waiting for child process
            }
        }
        else
        {
            perror("FORK ERROR :");
            exit(0);
        }
        fflush(stdout); // all Done
    }
    return 0;
}