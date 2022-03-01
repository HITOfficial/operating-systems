#ifdef MERGE_COMPARE_LIB
#define MERGE_COMPARE_LIB

typedef struct
{
    int blocks_number;
    char **block;
} blocks_of_lines;

void add_block_at_index(blocks_of_lines *blocks, char *block, int index);
void remove_block_at_index(blocks_of_lines *blocks, char *block, int index);
// crate merged blocks
block_of_lines create_merge_blocks(char *a, char *b);
// define sequence

// create tmp merged blocks file
int number_of_lines_in_block(char *)

#endif