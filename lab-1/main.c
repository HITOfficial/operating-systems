#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// GLOBAL VARIABLES
int BUFFER_SIZE = 255;

typedef struct
{
    FILE **fp;
    int lines;
    int characters;
    int words;
} block_s;

typedef struct
{
    // block_s **blocks;
    int actual_block;
    int blocks_number;
    block_s **blocks;
} main_table_s;

int add_file(main_table_s *main_table, char *file_path)
{
    (*main_table).actual_block++;
    FILE *fp = fopen(file_path, "r");
    char buffer[BUFFER_SIZE];
    int lines = 0;
    int characters = 0;
    int words = 0;
    int index;

    while (!feof(fp))
    {
        // reading full line
        fgets(buffer, BUFFER_SIZE, fp);
        index = 0;
        while (index < BUFFER_SIZE && buffer[index] != '\n' && buffer[index] != '\0')
        {
            while (index < BUFFER_SIZE && buffer[index] != ' ' && buffer[index] != '\n' && buffer[index] != '\0')
            {
                characters++;
                index++;
            }
            // checking if was ended by a space to increment number of characters in line
            if (index < BUFFER_SIZE && buffer[index] == ' ')
            {
                characters++;
            }
            words++;
            index++;
        }
        lines++;
    }

    // getting back with the pointer to start of file
    fseek(fp, 0, SEEK_SET);

    // updating block struct values
    (*(*main_table).blocks[(*main_table).actual_block]).lines = lines;
    (*(*main_table).blocks[(*main_table).actual_block]).words = words;
    (*(*main_table).blocks[(*main_table).actual_block]).characters = characters;
    (*(*main_table).blocks[(*main_table).actual_block]).fp = calloc(1, sizeof(FILE *));
    (*(*main_table).blocks[(*main_table).actual_block]).fp = &fp;

    return (*main_table).actual_block;
}

main_table_s *create_main_table(int blocks_number)
{
    main_table_s *main_table = calloc(1, sizeof(main_table_s));
    // array of pointers
    (*main_table).blocks = calloc(blocks_number, sizeof(block_s *));
    (*main_table).blocks_number = blocks_number;
    (*main_table).actual_block = -1;

    // declaring the address of a single pointer
    for (int i = 0; i < (*main_table).blocks_number; i++)
    {
        (*main_table).blocks[i] = calloc(1, sizeof(block_s));
    }

    return main_table;
}

int main(int argc, char **argv)
{
    main_table_s *main_table = create_main_table(2);

    char *path1 = "./sample.txt";
    char *path2 = "./sample2.txt";

    int index = add_file(main_table, path1);
    index = add_file(main_table, path2);

    printf("%d\n", (*(*main_table).blocks[1]).lines);
    printf("%d\n", (*(*main_table).blocks[0]).lines);

    return 0;
}