#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#define MAX_DIR 255

void creatEnv();

/**
 * 파일을 복사하여 사본을 생성한다. 
 * @param buf   경로명을 저장할 버퍼 변수.
 * @param size  버퍼 변수의 크기
 * @return 버퍼 변수의 시작 주소, 즉 buf
 */
char *mygetcwd(char *buf2, size_t size)
{
  DIR *dp;
  DIR *root_dp;
  struct dirent *dent, *root_dent;
  char buf[MAX_DIR] = "..";
  int now_inode, save_inode, root_inode, i = 0, j = 0, k = 0;
  ;
  memset(buf2, '\0', MAX_DIR);
  dp = opendir(".");
  root_dp = opendir("/");

  if (dent = readdir(dp))
  {
    save_inode = (int)dent->d_ino;
    now_inode = (int)dent->d_ino;
  }

  if (root_dent = readdir(root_dp))
  {
    root_inode = (int)root_dent->d_ino;
  }
  while (root_inode != save_inode)
  {
    dp = opendir(buf);
    if (dent = readdir(dp))
    {
      save_inode = now_inode;
      now_inode = (int)dent->d_ino;
    }
    while ((dent = readdir(dp)))
    {
      if ((int)dent->d_ino == save_inode)
      {
        strcat(buf2, dent->d_name);
        strcat(buf2, "/");
        break;
      }
    }
    strcat(buf, "/..");
    i++;
  }
  for (i = 255; i >= 0; --i)
  {
    if (buf2[i] == '/')
    {
      buf2[i] = '\0';
      buf2[i - 1] = '\0';
      buf2[i - 2] = '\0';
      break;
    }
  }

  memset(buf, '\0', MAX_DIR);
  for (i = 255; i >= 0; --i)
  {
    if (buf2[i] == '/')
    {
      if (buf2[i + 1] != '\0')
      {
        k = i + 1;
        buf[j++] = '/';
        while (buf2[k] != '/')
        {
          buf[j++] = buf2[k++];
        }
      }
    }
    else if ((buf2 + i) == buf2)
    {
      buf[j++] = '/';
      k = 0;
      while (buf2[k] != '/')
      {
        buf[j++] = buf2[k++];
      }
    }
  }
  closedir(dp);
  closedir(root_dp);
  strcpy(buf2, buf);
  return buf2;
}

int main(void)
{
  pid_t pid;
  int status;
  char buf[255];

  creatEnv();
  chdir("dir/sub");

  printf("original func: %s\n", getcwd(NULL, 0));
  printf("mygetcwd func: %s\n", mygetcwd(buf, 255));

  return 0;
}

void creatEnv()
{
  mkdir("dir", 0755);
  mkdir("dir/sub", 0755);
  mkdir("dir/sub2", 0);

  creat("dir/a", 0755);
  creat("dir/b", 0755);
  creat("dir/sub/x", 0755);
  symlink("dir/a", "dir/sl");
  symlink("dir/x", "dir/dsl");
}
