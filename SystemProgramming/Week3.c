#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){
	int fd;
	mode_t old_mask, new_mask;
	old_mask = umask(022);
	fd = open("test", O_CREAT | O_WRONLY, 0666);
	printf("old_mask=%o\n", old_mask);

	new_mask = umask(old_mask);
	printf("new_mask=%o\n", new_mask);
	close(fd);
	return 0;
}