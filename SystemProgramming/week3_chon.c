#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>

int main(){
	struct passwd *u_info;
	u_info = getpwnam("testuser");
	chown("test", u_info->pw_uid, -1);
	return 0;
}