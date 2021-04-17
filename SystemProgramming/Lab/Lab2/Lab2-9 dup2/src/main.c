#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(void){
  int fd;
  if((fd = open("tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1){
    perror("creat");
    exit(1);
  }
  dup2(fd, 1);
  printf("DUP2: Standard Output Redirection\n");
  close(fd);
  return 0;
}

