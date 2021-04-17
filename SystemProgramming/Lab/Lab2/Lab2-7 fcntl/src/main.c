#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
void fatal(const char *str, int errno){
  perror(str);
  exit(errno);
}
int main(int argc, char *argv[]) {
  int accmode, val, fd;
  if((fd = open("tmp", O_RDWR)) == -1)
    fatal("open", 1);
  if ((val = fcntl(fd, F_GETFL, 0)) < 0)
    fatal("fcntl error for fd", 1);
  accmode = val & O_ACCMODE;
  if (accmode == O_RDONLY)
    printf("read only");
  else if (accmode == O_WRONLY)
    printf("write only");
  else if (accmode == O_RDWR)
    printf("read write");
  else {
     fprintf(stderr, "unkown access mode");
    exit(1);
  }
  val |= O_APPEND;
  if(fcntl(fd, F_SETFL, val) == -1)
    fatal("fcntl setfl", 1);
  if(write(fd, "abcdefghij", 10) != 10)
    fatal("write", 1);

  if (val & O_APPEND)
    printf(", append");
  if (val & O_NONBLOCK)
    printf(", nonblocking");
  if (val & O_SYNC)
    printf(", synchronous writes");
  putchar('\n');
  close(fd);
}

