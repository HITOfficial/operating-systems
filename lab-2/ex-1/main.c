#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

int BUFFER_SIZE = 256;


int main(int argc, char *argv[])
{
    char path_a[100];
    char path_b[100];

    if (argc >= 3)
    {
        strncpy(path_a, argv[1],100);
        strncpy(path_b, argv[2],100);
    }
    else if (argc == 1)
    {
        printf("relative path of file to copy: ");
        fgets(path_a,sizeof(path_a),stdin);
        printf("relative path of file where will be results: ");
        fgets(path_b,sizeof(path_b),stdin);
        // removing \n from args to path
        path_a[strlen(path_a) -1] = '\0';
        path_b[strlen(path_b) -1] = '\0';

    }
    else {
        strncpy(path_a, argv[1],100);
        printf("relative path of file where will be results:");
        fgets(path_b,sizeof(path_b),stdin);
        // removing \n
        path_b[strlen(path_b) -1] = '\0';

    }
    


    FILE *fp_a = fopen(path_a, "r");
    FILE *fp_b = fopen(path_b, "w");
    
    char buffer[BUFFER_SIZE];

    while (!feof(fp_a)) {
        fgets(buffer, BUFFER_SIZE, fp_a);
        int flag = 0;
        int i = 0;
        while (i < BUFFER_SIZE) {
            if (buffer[i] == '\0'){
                break;
            } 
            else if (buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\t' && buffer[i] != ' ' && buffer[i] != '\0'){
                flag = 1;
                break;
            }
            i ++;
        }
        if (flag) {
            fprintf(fp_b,"%s",buffer);
        }
    }

    fclose(fp_a);
    fclose(fp_b);

    return 0;
}