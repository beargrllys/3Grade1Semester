#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int fd, n;
	char buf[10];
	mode_t mode;
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if((fd = open("test", O_CREAT, mode)) == -1){
		perror("creat");
		exit(1);
	}

	if((n = read(fd, buf, 8)) == -1){
		perror("read");
		exit(1);
	}
	buf[n] = '\0';
	printf("n = %d, buf = %s\n", n, buf);
	close(fd);
	return 0;
}
