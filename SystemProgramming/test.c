#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#define MAX_DIR 1024

int main(void)
{
    DIR *dp;
    DIR *root_dp;
    struct dirent *dent, *root_dent;
    char buf2[MAX_DIR], buf[MAX_DIR] = "..";
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
    for (i = 1024; i >= 0; --i)
    {
        if (buf2[i] == '/')
        {
            buf2[i] = '\0';
            buf2[i - 1] = '\0';
            buf2[i - 2] = '\0';
            break;
        }
    }

    printf("%s\n", buf2);
    memset(buf, '\0', MAX_DIR);
    for (i = 1024; i >= 0; --i)
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
    printf("%s\n", buf);
    closedir(dp);
    closedir(root_dp);
    return 0;
}