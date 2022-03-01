#include <stdio.h>
#include <string.h>

int BUFFER_SIZE = 255;

typedef struct
{
    row_s *rows;
    char rows_number;
} block_s;

typedef struct
{
    char *content;
    int length;
} row_s;

typedef struct
{
    block_s blocks *
} main_table_s;

void merge_files(char *a_file_path, char *b_file_path)
{
    FILE *a_fp = fopen(a_file_path, "r");
    FILE *b_fp = fopen(b_file_path, "r");

    fclose(a_fp);
    fclose(b_fp);
}

//
block_s create_block(FILE fp)
{
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
}

void add_row(block_t block)

    int main()
{
    FILE *fp = fopen("sample.txt", "r");
    char buffer[50];
    int lines = 0;
    while (!feof(fp))
    {
        fgets(buffer, 50, fp);
        lines++;
    }

    char *t = "simple tekst";
    printf("%d", strlen(t));

    printf("%d", lines);
    return 0;
    fclose(fp);
    // return line_length(t, 0);
}