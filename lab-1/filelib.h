#ifndef FILELIB_H
#define FILELIB_H

typedef struct
{
    char **content;
    int lines;
    int characters;
    int words;
} block_s;

typedef struct
{
    int actual_block;
    int blocks_number;
    block_s **blocks;
} main_table_s;

int add_file(main_table_s *main_table, char *file_path);
void free_block_content(block_s *block);
void remove_block(main_table_s *main_table, int index);
void delete_main_table(main_table_s *main_table);
void block_l(main_table_s *main_table, int index);
void block_w(main_table_s *main_table, int index);
void block_c(main_table_s *main_table, int index);
main_table_s *create_main_table(int blocks_number);

#endif // end of FILELIB_H