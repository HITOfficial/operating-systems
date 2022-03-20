#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


struct file_types {
    int dir;
    int reg;
    int sock;
    int blk;
    int fifo;
    int chr;
    int lnk;
};


void update_types_counter(struct file_types *types,struct dirent *entity){
    switch (entity->d_type)
    {
    case 4:
        types->dir ++;
        break;
    case 12:
        types->sock ++;
        break;
    case 8:
        types->reg ++;
        break;
    case 6:
        types->blk ++;
        break;
    case 1:
        types->fifo ++;
        break;
    case 10:
        types->lnk ++;
        break;
    case 2:
        types->chr ++;
        break;
    default:
        break;
    }
}

char *get_type(struct dirent *entity) {
switch (entity->d_type)
    {
    case 4:
        return "-dir";
    case 12:
        return "-sock";
    case 8:
        return "-reg";
    case 6:
        return "-block dev";
    case 1:
        return "-fifo";
    case 10:
        return "-slink";
    case 2:
        return "-char dev";
    default:
        return "";
    }
}

void file_info(char path[], struct stat buffer, struct dirent *entity){
    printf("path: %s| type: %s| protection: %d| size: %ld| last acces: %ld| last modification: %ld| \n",path, get_type(entity),buffer.st_mode,buffer.st_size,buffer.st_atime, buffer.st_mtime);
}

void summary_files_info(struct file_types *ftp){
    printf("DIR: %d| SOCK: %d| REG: %d| BLK: %d| FIFO: %d| LNK: %d| CHR: %d| \n",ftp->dir,ftp->sock,ftp->reg,ftp->blk,ftp->fifo,ftp->lnk,ftp->chr);

}

void list_files(char *dirname, struct file_types *ftp){
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        printf("path not found");
        return ;
    }
    
    struct stat buffer;
    struct dirent *entity;

    entity = readdir(dir);
    while(entity != NULL) {
        // condition to not check infinite same directories
        if (strcmp(entity->d_name,".") != 0 && strcmp(entity->d_name,"..") != 0) {
            // update counter of files
            char path[100] = {0};
            // creating path
            strcat(path,dirname); 
            strcat(path,"/");
            strcat(path,entity->d_name);
            stat(path,&buffer);
            update_types_counter(ftp,entity);
            file_info(path,buffer,entity);
            if(entity->d_type == DT_DIR && strcmp(entity->d_name,".") != 0 && strcmp(entity->d_name,"..") != 0){
                list_files(path,ftp);
            }
        }
        entity = readdir(dir);
    }
    closedir(dir);

}


int main(int argc, char const *argv[])
{
    if (argc <2) {
        printf("pass argument");
        return 0;
    }
    struct file_types types = {1,0,0,0,0,0,0}; 
    struct file_types *ftp = &types;
    char *fullpath = realpath(argv[1],NULL);
    list_files(fullpath, ftp);
    summary_files_info(ftp);
    return 0;
}
