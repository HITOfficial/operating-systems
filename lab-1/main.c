#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "filelib.c"

int main(int argc, char **argv)
{
    main_table_s *main_table = create_main_table(3);

    char *path1 = "./sample.txt";
    char *path2 = "./sample2.txt";
    char *path3 = "./sample3.txt";

    add_file(main_table, path1);
    add_file(main_table, path2);
    add_file(main_table, path3);

    delete_main_table(main_table);
    return 0;
}
