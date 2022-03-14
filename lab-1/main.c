#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include "filelib.c"
#include "filepath.c"

static clock_t start_time;
static clock_t end_time;
static struct tms start_cpu;
static struct tms end_cpu;

void start_clock()
{
    start_time = times(&start_cpu);
}

void end_clock()
{
    printf("NEW LINE \n ");
    end_time = times(&end_cpu);
    double x = end_cpu.tms_utime;
    printf("REAL TIME: %f, USER TIME %f, SYSTEM TIME %f",
           (double)(end_time - start_time),
           (double)(end_cpu.tms_utime - start_cpu.tms_utime),
           (double)(end_cpu.tms_stime - start_cpu.tms_stime));
}

int main(int argc, char **argv)
{
    int s = 5;
    int m = 3;
    int l = 2;
    // char *path1 = "./large/1.txt";
    // char *path2 = "./sample2.txt";
    // char *path3 = "./sample3.txt";

    // struct tms t_start;
    // clock_t x = time(&t_start);
    int cycles[3] = {1,
                     10,
                     100};

    int counter = 0;
    for (int i = 0; i < 3; i++)
    {
        counter += cycles[i] * (s + m + l);
    }
    main_table_s *main_table = create_main_table(counter);

    // clock_t start = clock();
    start_clock();
    printf("TESTING FILES: %d SMALL, %d MEDIUM, %d LARGE \n", s, m, l);
    end_clock();
    start_clock();
    printf("NUMBER OF CYCLES IN OPERATIONS OF THIS FILES: ");
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", cycles[i]);
    }

    printf("\n");

    for (int i = 0; i < 3; i++)
    {
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
    }
    clock_t end = clock();
    printf("ADDING ALL FILES TIME: ");
    printf("\n");

    end_clock();
    start_clock();

    for (int i = 1; i < 3; i++)
    {
        for (int j = 0; j < cycles[i]; j++)
        {
            for (int k = 0; k < (s + m + l); k++)
            {
                remove_block(main_table, 0);
            }
        }
    }

    // int id = 6;
    // main_table_s *main_table = create_main_table(id);

    // for (int i = 0; i < id; i++)
    // {
    //     add_file(main_table, small_path[0]);
    // }

    // for (int i = 0; i < id; i++)
    // {
    //     printf("%d", i);
    //     remove_block(main_table, 0);
    // }

    delete_main_table(main_table);
    printf("REMOVING ALL FILES TIME:");
    end_clock();

    return 0;
}