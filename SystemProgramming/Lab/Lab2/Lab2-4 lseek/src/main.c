#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	int fd;

	char buf1[] = "abcdefghij";
	char buf2[] = "ABCDEFGHIJ";

	if((fd = creat("file.hole", 0640)) < 0){
		perror("creat");
		exit(1);
	}
	if(write(fd, buf1, 10) != 10){
		perror("write1");
		exit(1);
	} // 10
	if(lseek(fd, 40, SEEK_SET) == -1){
		perror("lseek");
		exit(1);
	} // 40
	if(write(fd, buf2, 10) != 10){
		perror("write2");
		exit(1);
	} // 50
	close(fd);
	return 0;
}
