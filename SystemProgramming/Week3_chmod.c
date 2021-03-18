#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){
	if(chmod("test", S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR) < 0){
		perror("chmod error: ");
	}
	return 0;
}