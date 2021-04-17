#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int fd;
	mode_t mode;
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if((fd = open("test", O_TRUNC, mode)) == -1){
		perror("creat");
		exit(1);
	}
	close(fd);
	return 0;
}
