#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(void){
  int fd, n;
  off_t start, cur;
  char buf[256];
  if((fd = open("test.txt", O_RDONLY)) == -1){
    perror("open");
    exit(1);
  }
  start = lseek(fd, 0, SEEK_CUR);
  n = read(fd, buf, 255);
  buf[n] = '\0';
  printf("Offset start = %d, Read str = %s, n = %d\n", (int)start, buf, n);
  cur = lseek(fd, 0, SEEK_CUR);
  printf("Offset cur = %d\n", (int)cur);

  start = lseek(fd, 5, SEEK_SET);
  n = read(fd, buf, 255);
  buf[n] = '\0';
  printf("Offset start = %d, Read str = %s, n = %d\n", (int)start, buf, n);

  close(fd);
  return 0;
}
