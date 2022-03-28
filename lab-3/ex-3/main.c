#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

int BUFFER_SIZE = 256;

int find_pivot_in_file(char* path, char* pivot) {
    int pivot_length = strlen(pivot);
    FILE *fp = fopen(path,"r");
    char buffer[BUFFER_SIZE];
    char tmp_pivot[pivot_length+1];
    int file_with_pivot = 0;
    // comparing every [pivot length] part of line with pivot, to check if they are the same 
    int flag = 0;
    while (!feof(fp)) {
        fgets(buffer,BUFFER_SIZE, fp);
        int i = 0;
        while (i < BUFFER_SIZE - pivot_length && buffer[i] != '\n' && buffer[i] != '\0'){
            // index of char comparing from, number of char to compare
            strncpy(tmp_pivot,buffer+ i, pivot_length);
            // Replaced strcmp with strncmp couse of \0 at the end;
            if (strncmp(pivot,tmp_pivot,pivot_length) == 0) {
                flag = 1;
                break;
            }
            i++;
        }
        if (flag) {
            break;  
        } 
    }
    fclose(fp);
    return flag;
}




void list_files_find_pivot(char *dirname, char* pivot,FILE *fp,int depth, int max_depth)
{
    if (depth >= max_depth){
        return ;
    }

    DIR *dir = opendir(dirname);
    if (dir == NULL)
    {
        printf("path not found");
        return;
    }

    struct dirent *entity;

    entity = readdir(dir);
    while (entity != NULL)
    {
        // condition to not check infinite same directories
        if (strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)
        {
            // update counter of files
            char path[100] = {0};
            // creating path
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            // file extension 
            char ext[10] = {0};
            int path_len = strlen(path);
            strncpy(ext,path+path_len-4,4);
            if (strcmp(ext,".txt")== 0){
                char pth[100] = {0};
                strcpy(pth,path);
                if (find_pivot_in_file(path,pivot)){
                    printf("[PID]:%d path:%s \n",getpid(),path);
                    fprintf(fp,"[PID]:%d path:%s \n",getpid(),path);
                }
            }
            if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)
            {
                int id = fork();
                if (id ==0) {
                    list_files_find_pivot(path, pivot,fp,depth+1,max_depth);
                    while(wait(NULL) != -1);
                    exit(0);
                }
                while(wait(NULL) != -1);
            }
        }
        entity = readdir(dir);
    }
    closedir(dir);
}


int main(int argc, char *argv[])
{
    if(argc < 4){
        printf("to LESS ARGS");
        return 0;
    }

    char *path = argv[1];
    char *pivot = argv[2];
    int depth = atoi(argv[3]);

    // creating 
    FILE *fp = fopen("results.txt","w+");
    int id = fork();
    if(id==0){
        list_files_find_pivot(path,pivot,fp,0,depth);
        exit(0);
    }
    // waiting to end children exec
    while(wait(NULL) != -1);
    return 1;
}

