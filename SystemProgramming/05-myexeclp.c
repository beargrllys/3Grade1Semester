#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#define MAXARGS 31
#define MAXFILENAME 1024
#define MAXPATHSTR 2048

extern char **environ;
/**
 * 첫 번째 인자인 file 프로그램을 뒤에 따르는 인자들을 이용해 실행한다.
 * @param file	실행할 프로그램의 이름
 * @param args	stdarg(3)을 통해 처리할 인자들
 * @return 실패했을 경우 -1, 성공 시 반환 값 없음
 */
int myexeclp(char *file, const char *args, ...)
{
  int i = 1;
  char *value, *param[MAXARGS], comm;
  va_list ap;

  va_start(ap, args);
  value = getcwd(NULL, 0);
  strcat(value, "/");
  strcat(value, file);

  if (value == NULL)
  {
    fprintf(stderr, "There is no env path.\n");
    return -1;
  }

  if (access(value, X_OK) == -1)
  {
    fprintf(stderr, "The file is unexecutable\n");
    return -1;
  }
  param[0] = file;
  while (1)
  {
    param[i] = va_arg(ap, char *);
    if (param[i] == 0)
    {
      break;
    }
    i++;
  }

  if (execve(file, param, environ) == -1)
  {
    fprintf(stderr, "Exe error occur\n");
    return -1;
  }
  va_end(ap);
  return 0;
}

int main(void)
{
  char path[MAXPATHSTR];
  sprintf(path, "PATH=%s:%s\n", getcwd(NULL, 0), getenv("PATH"));
  putenv(path);
  // prepare the executable file named "hello"
  system("gcc -o hello hello.c");
  myexeclp("hello", "hello", "-a", "-b", "-c", (char *)0);

  return 0;
}