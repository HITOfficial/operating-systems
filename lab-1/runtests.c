#include "clock.c"
#include "filelib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int B_SIZE = 255;


void runtests(main_table_s *main_table, int cycles[3], int s, int m, int l, int c)
{
    // write on the end of file if exists and is not empty
    FILE *fpt = fopen("fileresults.txt", "ab");

    printf("TESTING FILES: %d SMALL, %d MEDIUM, %d LARGE", s, m, l);
    fprintf(fpt, "TESTING FILES: %d SMALL, %d MEDIUM, %d LARGE", s, m, l);
    printf("\nSMALL FILES PATH \n");
    fprintf(fpt, "\nSMALL FILES PATH \n");
    for (int i = 0; i < s; i++)
    {
        printf("%s", small_path[i]);
        fprintf(fpt,"%s", small_path[i]);
    }
    printf("\nMEDIUM FILES PATH \n");
    fprintf(fpt,"\nMEDIUM FILES PATH \n");
    for (int i = 0; i < m; i++)
    {
        printf("%s", medium_path[i]);
        fprintf(fpt,"%s", medium_path[i]);
    }
    printf("\nLARGE FILES PATH \n");
    fprintf(fpt,"\nLARGE FILES PATH \n");
    for (int i = 0; i < l; i++)
    {
        printf("%s", large_path[i]);
        fprintf(fpt,"%s", large_path[i]);
    }

    printf("\nNUMBER OF CYCLES IN OPERATIONS ON THIS FILES ");
    fprintf(fpt,"\nNUMBER OF CYCLES IN OPERATIONS ON THIS FILES ");
    for (int i = 0; i < c; i++)
    {
        printf("%d ", cycles[i]);
        fprintf(fpt,"%d ", cycles[i]);
    }
    printf("\nSTARTING\n");
    fprintf(fpt,"\nSTARTING\n");

    for (int i = 0; i < c; i++)
    {

        printf("CYCLE %d TIMES ", cycles[i]);
        fprintf(fpt,"CYCLE %d TIMES ", cycles[i]);
        start_clock();
        for (int j = 0; j < cycles[i]; j++)
        {
            // small
            for (int i = 0; i < s; i++)
            {
                add_file(main_table, small_path[i]);
            }
            // medium
            for (int i = 0; i < m; i++)
            {
                add_file(main_table, medium_path[m]);
            }
            // large
            for (int i = 0; i < l; i++)
            {
                add_file(main_table, small_path[l]);
            }
        }
        printf("\nADDING ALL FILES ");
        fprintf(fpt,"\nADDING ALL FILES ");
        end_clock(fpt);
        // removing
        start_clock();
        for (int j = 0; j < cycles[i]; j++)
        {
            for (int k = 0; k < (s + m + l); k++)
            {
                remove_block(main_table, 0);
            }
        }
        printf("REMOVING ALL FILES \n");
        fprintf(fpt,"REMOVING ALL FILES \n");
        end_clock(fpt);
    }
    printf("END OF TESTING");
    fprintf(fpt,"END OF TESTING");
    fclose(fpt);
}
