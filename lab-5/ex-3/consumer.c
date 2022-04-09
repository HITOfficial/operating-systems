#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRODUCER 10
#define LINE_LENGTH 512

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf( "Wrong number of args!\n");
        exit(1);
    }

    FILE *pipe_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w+");
    const int buffer_size = atoi(argv[3]);

    char array[PRODUCER][LINE_LENGTH] = {0};
    char tmp[PRODUCER * LINE_LENGTH] = "";
    char *buffer = calloc(buffer_size + 3, sizeof(char));
    while (fread(buffer, 1, buffer_size+2, pipe_file) > 0) {
        strcat(tmp, buffer);
    }

    char new_lines[] = "|\n";
    char* c;
    c = strtok(tmp, new_lines);
    int max_row = 0;
    while(c != NULL) {
      int row = atoi(c);
      c = strtok(NULL, new_lines);
      strcat(array[row], c);
      c = strtok(NULL, new_lines);

      if(row > max_row)
        max_row = row;
    }

    for(int i = 0; i <= max_row; i++) {
      fprintf(output_file, "%s\n", array[i]);
    }

    printf("consumed all information's");
    free(buffer);
    fclose(output_file);
    fclose(pipe_file);
}