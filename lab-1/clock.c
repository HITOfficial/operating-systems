#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>

static clock_t start_time;
static clock_t end_time;
static struct tms start_cpu;
static struct tms end_cpu;

void start_clock()
{
    start_time = times(&start_cpu);
}

void end_clock(FILE *fpt)
{
    end_time = times(&end_cpu);
    double x = end_cpu.tms_utime;
    printf("REAL TIME: %f  USER TIME: %f SYSTEM TIME: %f \n",
           (double)(end_time - start_time),
           (double)(end_cpu.tms_utime - start_cpu.tms_utime),
           (double)(end_cpu.tms_stime - start_cpu.tms_stime));
    fprintf(fpt,"REAL TIME: %f  USER TIME: %f SYSTEM TIME: %f \n",
           (double)(end_time - start_time),
           (double)(end_cpu.tms_utime - start_cpu.tms_utime),
           (double)(end_cpu.tms_stime - start_cpu.tms_stime));
}