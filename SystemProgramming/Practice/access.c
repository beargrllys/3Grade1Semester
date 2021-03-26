#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: a. out <pathname>");
        exit(1);
    }
    if (access(argv[1], R_OK) < 0) //읽기 권한 확인
        perror("access error");
    else
        printf("read access OK\n");

    if (open(argv[1], O_RDONLY) < 0) //
        perror("open error");
    else
        printf("open for reading OK\n");

    exit(0);
}

struct stat
{
    dev_t st_dev;             /* device number */
    ino_t st_ino;             /* inodenumber */
    mode_t st_mode;           /* file type, mode (permissions) */
    nlink_t st_nlink;         /* number of hard links */
    uid_t st_uid;             /* user ID of owner */
    gid_t st_gid;             /* group ID of owner */
    dev_t st_rdev;            /* device type for special files (if inodedevice) */
    off_t st_size;            /* total size, in bytes */
    unsigned long st_blksize; /* blocksizefor I/O */
    unsigned long st_blocks;  /* # of blocks allocated */
    time_t st_atime;          /* time of last access */
    time_t st_mtime;          /* time of last modification */
    time_t st_ctime;          /* time of last status change */
};