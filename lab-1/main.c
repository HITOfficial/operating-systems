#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "filelib.c"

char *small_path[10] = {
    "./small/1.txt",
    "./small/2.txt",
    "./small/3.txt",
    "./small/4.txt",
    "./small/5.txt",
    "./small/6.txt",
    "./small/7.txt",
    "./small/8.txt",
    "./small/9.txt",
    "./small/10.txt",
};
char *medium_path[10] = {
    "./medium/1.txt",
    "./medium/2.txt",
    "./medium/3.txt",
    "./medium/4.txt",
    "./medium/5.txt",
    "./medium/6.txt",
    "./medium/7.txt",
    "./medium/8.txt",
    "./medium/9.txt",
    "./medium/10.txt",
};
char *large_path[10] = {
    "./large/1.txt",
    "./large/2.txt",
    "./large/3.txt",
    "./large/4.txt",
    "./large/5.txt",
    "./large/6.txt",
    "./large/7.txt",
    "./large/8.txt",
    "./large/9.txt",
    "./large/10.txt",
};

int main(int argc, char **argv)
{
    main_table_s *main_table = create_main_table(3);

    char *path1 = "./large/1.txt";
    char *path2 = "./sample2.txt";
    char *path3 = "./sample3.txt";

    add_file(main_table, large_path[0]);
    add_file(main_table, path2);
    add_file(main_table, path3);
    printf("RUNED");
    delete_main_table(main_table);
    return 0;
}
