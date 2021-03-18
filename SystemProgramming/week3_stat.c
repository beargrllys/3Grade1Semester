#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

int main(){
	char *file_name = "test";
	struct stat file_info;
	struct passwd *my_passwd;
	struct group *my_group;
	mode_t file_mode;

	if(stat(file_name, &file_info) < 0){
		perror("stat: ");
		return -1;
	}

	file_mode = file_info.st_mode;
	printf("file name: %s\n", file_name);
	printf("===================================");
	printf("file type: ");

	if(S_ISREG(file_mode)){printf("regular file\n");}
	else if(S_ISLNK(file_mode)){printf("Symbolic link\n");}
	else if(S_ISDIR(file_mode)){printf("Directory\n");}
	else if(S_ISCHR(file_mode)){printf("Character device\n");}
	else if(S_ISBLK(file_mode)){printf("Block device\n");}
	else if(S_ISFIFO(file_mode)){printf("Fifo file\n");}
	else if(S_ISSOCK(file_mode)){printf("Socket file\n");}

	my_passwd = getpwuid(file_info.st_uid);
	my_group = getgrgid(file_info.st_gid);

	printf("OWNER: %s\n", my_passwd->pw_name);
	printf("GROUP: %s\n", my_group->gr_name);
	printf("File size is: %ld\n", file_info.st_size);
	printf("Number of hard links: %ld\n", file_info.st_nlink);

	return 0;
}