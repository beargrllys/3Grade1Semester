#include <stdio.h>
#include <unistd.h>

int main(){
	if(access("test", F_OK) != 0){
		perror("File is not exist");
	}
	else{
		printf("File is exist\n");
		if(access("test", R_OK | W_OK) != 0){
			perror("This file has no Read Write permission\n");
		} 
		else{
			printf("This file has Read Write permission\n");
		}
	}
	return 0;
}