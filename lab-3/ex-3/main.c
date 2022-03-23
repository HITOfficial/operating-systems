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
    char tmp_pivot[BUFFER_SIZE];
    int file_with_pivot = 0;
    // comparing every [pivot length] part of line with pivot, to check if they are the same 
    int flag = 0;
    while (!feof(fp)) {
        fgets(buffer,BUFFER_SIZE, fp);
        int i = 0;
        while (i < BUFFER_SIZE - pivot_length && buffer[i] != '\n' && buffer[i] != '\0'){
            // index of char comparing from, number of char to compare
            strncpy(tmp_pivot,buffer+ i, pivot_length);
            if (strcmp(pivot,tmp_pivot) == 0) {
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

    struct stat buffer;
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
            stat(path, &buffer);
            // checking if regular file
            if (entity->d_type == 8){
                int len = strlen(path);
                printf("checking: %s pid: %d \n", path,getpid());
                // checking if found pivot in file
                if (find_pivot_in_file(path,pivot) == 1) {
                    printf("%s \n", path);
                    // fprintf("PID: %d  PATH: %s \n", getpid(), path);
                }
            }
            if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)
            {
                list_files_find_pivot(path, pivot,fp,depth+1,max_depth);
            }
        }
        entity = readdir(dir);
    }
    closedir(dir);
}


int main(int argc, char const *argv[])
{
    char *pivot = "data";
    char *path = "./t.txt";
    char *p = ".";
    // creating 
    FILE *fp = fopen("results.txt","w+");
        list_files_find_pivot(p,pivot,fp,0,5);

    printf("%d",find_pivot_in_file(path,pivot));
    return find_pivot_in_file(path, pivot);
}


