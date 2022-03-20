#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ftw.h>
#include "fileprop.c"

struct file_types TYPES = {1, 0, 0, 0, 0, 0, 0};
struct file_types *FT = &TYPES;

char *file_type(int flags)
{
    switch (flags)
    {
    case FTW_D || FTW_DNR:
        FT->dir++;
        return ("-dir");
    case FTW_F:
        FT->reg++;
        return ("-reg");
    case FTW_SL:
        FT->lnk;
        return ("-slink");
    default:
        return ("-unknown");
    }
}

int rec_file_info(const char *path, const struct stat *buffer,
                  int flag, struct FTW *ftwbuf)
{
    printf("path: %s| type: %s| protection: %d| size: %ld| last acces: %ld| last modification: %ld| \n", path, file_type(flag), buffer->st_mode, buffer->st_size, buffer->st_atime, buffer->st_mtime);
    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("pass argument");
        return 0;
    }
    char *startpath = realpath(argv[1], NULL);
    int flags = FTW_CHDIR | FTW_DEPTH | FTW_MOUNT;
    nftw(startpath, rec_file_info, 20, flags);
    summary_files_info(FT);
}
