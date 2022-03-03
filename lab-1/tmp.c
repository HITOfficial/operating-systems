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
    block_s **blocks;
    int blocks_number;
} main_table_s;

// void remove_block(main_table_s *main_table, int block_index)
// {
//     (*main_table).blocks[]
//         // closing file
//         fclose(*(*block).fp);
//     free(block);
// }

int main(int argc, char **argv)
{
    main_table_s *main_table = calloc(1, sizeof(main_table_s));
    (*main_table).blocks = calloc(1, sizeof(block_s *));
    (*main_table).blocks_number = 1;

    FILE *fp = fopen("./sample.txt", "r");
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

    // connecting pointers to pointers
    block_s *block = calloc(1, sizeof(block_s));
    (*block).lines = lines;
    (*block).words = words;
    (*block).characters = characters;
    (*block).fp = calloc(1, sizeof(FILE *));
    (*block).fp = &fp;

    (*main_table).blocks = &block;
    // remove_block(main_block, 0);
    // fclose(*(*block).fp);

    return 0;
}

// int block_lines(main_table_s *main_table, int index)
// {
//     (*main_table).
// }

// delete_main_table(main_table_s *main_table)
// {
//     for (int i = 0; i < (*main_table).blocks_number; i++)
//     {
//         remove_block((*main_table))
//     }
// }