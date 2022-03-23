#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int BUFFER_SIZE = 256;

int main(int argc, char *argv[])
{
    char path_a[100];
    char path_b[100];
    int buffer_pow_2 = BUFFER_SIZE * BUFFER_SIZE;

    if (argc >= 3)
    {
        strncpy(path_a, argv[1], 100);
        strncpy(path_b, argv[2], 100);
    }
    else if (argc == 1)
    {
        printf("relative path of file to copy: ");
        fgets(path_a, sizeof(path_a), stdin);
        printf("relative path of file where will be results: ");
        fgets(path_b, sizeof(path_b), stdin);
        // removing \n from args to path
        path_a[strlen(path_a) - 1] = '\0';
        path_b[strlen(path_b) - 1] = '\0';
    }
    else
    {
        strncpy(path_a, argv[1], 100);
        printf("relative path of file where will be results:");
        fgets(path_b, sizeof(path_b), stdin);
        // removing \n
        path_b[strlen(path_b) - 1] = '\0';
    }

    int fd_a = open(path_a, O_RDONLY);
    int fd_b = open(path_b, O_CREAT | O_WRONLY, 7777);

    char buffer[buffer_pow_2];

    read(fd_a, buffer, buffer_pow_2);

    int i = 0;
    int line_start;
    while (i < buffer_pow_2 && buffer[i] != '\0')
    {
        int flag = 0;
        line_start = i;
        while (i < buffer_pow_2 && buffer[i] != '\0' && buffer[i] != '\n')
        {
            if (buffer[i] != '\r' && buffer[i] != '\t' && buffer[i] != ' ' && buffer[i] != '\n')
            {
                flag = 1;
            }
            i++;
        }
        if (flag)
        {
            if (buffer[i] == '\n')
            {
                write(fd_b, buffer + line_start, i + 1 - line_start);
            }
            else
            {
                // checking if it is end of a file '\0'
                write(fd_b, buffer + line_start, i - line_start);
            }
        }
        i++;
    }

    close(fd_a);
    close(fd_b);

    return 0;
}