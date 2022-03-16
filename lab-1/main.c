#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "filelib.h"
#include "filepath.c"
#include "runtests.c"

int main(int argc, char *argv[])
{
    int s = (int)atoi(argv[1]);
    int m = (int)atoi(argv[2]);
    int l = (int)atoi(argv[3]);
    int c = (int)atoi(argv[4]);

    int cycles[c];

    for (int i = 5; i < argc; i++)
    {
        cycles[i - 5] = (int)atoi(argv[i]);
    }

    int counter = 0;
    for (int i = 0; i < c; i++)
    {
        counter += cycles[i] * (s + m + l);
    }

    main_table_s *main_table = create_main_table(counter);

    runtests(main_table, cycles, s, m, l, c);

    delete_main_table(main_table);
    return 0;
}