#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char **argv){
	int i; 
	char *file_name;
	struct stat file_info;
	mode_t file_mode;


	for(int i=1; i<argc; i++){

		file_name = argv[i];

		if(stat(file_name, &file_info) < 0){
			perror("stat: ");
			return -1;
		}

		file_mode = file_info.st_mode;

		if(S_ISDIR(file_mode)){
			if(chmod(argv[i], S_IRUSR | S_IRGRP | S_IROTH) < 0){
				perror("You have no right that modifies READ permission\n");
			}
			else{
				if(chmod(argv[i], S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH) < 0){
					perror("You have no right that modifies EXEC permission\n");
				}
				else{
					printf("Directory file \"%s\": permission is done!\n", argv[i]);
				}
			}		
		}
		else{
			if(chmod(argv[i], S_IRUSR | S_IRGRP | S_IROTH) < 0){
				perror("You have no right that modifies READ permission\n");
			}
			else{
				printf("Regular file \"%s\": get READ Permission\n", argv[i]);

				if((file_info.st_mode & S_IXUSR) || (file_info.st_mode & S_IXGRP) || (file_info.st_mode & S_IXOTH)){
					if(chmod(argv[i], S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH) < 0){
						perror("You have no right that modifies EXEC permission\n");
					}
					else{
						printf("Regular file \"%s\" get EXEC permission\n", argv[i]);
					}
				}
				else{
					perror("No one has EXEC permission.\n");
				}
			}
		}
		printf("\n\n");
	}

}