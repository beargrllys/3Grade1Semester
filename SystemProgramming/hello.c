#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int f[3] ={0};
  int opt;
  while( (opt = getopt(argc, argv, "abc")) != -1) {
    switch(opt){
      case 'a':
        f[0] = 1; break;
      case 'b':
        f[1] = 1; break;
      case 'c':
        f[2] = 1; break;
      default:
        printf("unknown flag %c\n", optopt);
        break;
    }
  }  

  if(f[0]) printf("Hello World\n");
  if(f[1]) printf("Hi, Bixby\n");
  if(f[2]) printf("OK, Google\n");  
  return 0;
}