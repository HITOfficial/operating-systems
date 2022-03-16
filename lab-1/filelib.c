#include "filelib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// GLOBAL VARIABLES
int BUFFER_SIZE = 255;

// adding new block file
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
                index++;
            }
            words++;
        }
        lines++;
    }

    // getting back with the pointer to start of file
    fseek(fp, 0, SEEK_SET);

    // updating block struct values
    (*(*main_table).blocks[(*main_table).actual_block]).lines = lines;
    (*(*main_table).blocks[(*main_table).actual_block]).words = words;
    (*(*main_table).blocks[(*main_table).actual_block]).characters = characters;

    // locking memory to memorize lines
    (*(*main_table).blocks[(*main_table).actual_block]).content = calloc(lines, sizeof(char *));

    for (int i = 0; i < lines; i++)
    {
        (*(*main_table).blocks[(*main_table).actual_block]).content[i] = calloc(BUFFER_SIZE, sizeof(char));
    }

    for (int i = 0; i < lines; i++)
    {
        fgets(buffer, BUFFER_SIZE, fp);
        memcpy((*(*main_table).blocks[(*main_table).actual_block]).content[i], buffer, BUFFER_SIZE);
    }

    fclose(fp);
    return (*main_table).actual_block;
}

// free block from memory
void free_block_content(block_s *block)
{
    for (int i = 0; i < (*block).lines; i++)
    {
        free((*block).content[i]);
    }
}

// removing single block
void remove_block(main_table_s *main_table, int index)
{
    if (index >= 0 && index < (*main_table).blocks_number)
    {
        free_block_content((*main_table).blocks[index]);
        free((*main_table).blocks[index]);

        // moving backward every block after this index
        for (int i = index + 1; i < (*main_table).blocks_number; i++)
        {
            (*main_table).blocks[i - 1] = (*main_table).blocks[i];
        }
        (*main_table).blocks_number--;
        (*main_table).actual_block--;
    }
}

// deleting main table
void delete_main_table(main_table_s *main_table)
{
    // after removing block i, every block after him is changing position by one backward;
    for (int i = 0; i < (*main_table).blocks_number; i++)
    {
        remove_block(main_table, 0);
    }
    free((*main_table).blocks);
    free(main_table);
}

// printing block properties
void block_l(main_table_s *main_table, int index)
{
    if (index >= 0 && index < (*main_table).blocks_number)
    {
        printf("%d\n", (*(*main_table).blocks[index]).lines);
    }
}

void block_w(main_table_s *main_table, int index)
{
    if (index >= 0 && index < (*main_table).blocks_number)
    {
        printf("%d\n", (*(*main_table).blocks[index]).words);
    }
}

void block_c(main_table_s *main_table, int index)
{
    if (index >= 0 && index < (*main_table).blocks_number)
    {
        printf("%d\n", (*(*main_table).blocks[index]).characters);
    }
}

// creating empty main table
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