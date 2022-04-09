#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    
    if (argc < 5) {
        printf( "Wrong number of args!\n");
        exit(1);
    }

    FILE *pipe_file = fopen(argv[1], "w");
    FILE *input_file = fopen(argv[2], "r");
    char* row = malloc(strlen(argv[3] + 1)*sizeof(char));
    row = argv[3];
    const int buffer_size = atoi(argv[4]);

    char *buffer = calloc(buffer_size + 1, sizeof(char));
    while (fread(buffer, 1, buffer_size, input_file) > 0) {
        sleep(rand() % 2 + 1);
        printf("Producing [row] %s [buffer] %s \n", row, buffer);
        fprintf(pipe_file, "%s|%s\n", row, buffer);
        fflush(pipe_file);
    }

    free(buffer);
    fclose(input_file);
    fclose(pipe_file);
}