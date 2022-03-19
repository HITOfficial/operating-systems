#include <stdio.h>
#include <sys/times.h>
#include <time.h>
#include <string.h>


int BUFFER_SIZE = 256;

static clock_t start_time;
static clock_t end_time;
static struct tms start_cpu;
static struct tms end_cpu;

void start_clock()
{
    start_time = times(&start_cpu);
}

char * end_clock(char str[])
{
    end_time = times(&end_cpu);
    printf("REAL TIME: %f  USER TIME: %f SYSTEM TIME: %f \n",
           (double)(end_time - start_time),
           (double)(end_cpu.tms_utime - start_cpu.tms_utime),
           (double)(end_cpu.tms_stime - start_cpu.tms_stime));
    
    sprintf(str, "REAL TIME: %f  USER TIME: %f SYSTEM TIME: %f \n",
            (double)(end_time - start_time),
            (double)(end_cpu.tms_utime - start_cpu.tms_utime),
            (double)(end_cpu.tms_stime - start_cpu.tms_stime));
}


int main(int argc, char const *argv[])
{
    if (argc < 3) {
        printf("to less args");
        return 0;

    }
    char path[100];
    strncpy(path, argv[1],100);
    char pivot = argv[2][0];
    start_clock();
    char buffer[BUFFER_SIZE*BUFFER_SIZE];
    int buffer_pow_2 = BUFFER_SIZE * BUFFER_SIZE;
    FILE *fp;
    fp = fopen(path, "r");

    FILE *measurement_p;
    measurement_p = fopen("lib_measurement.txt","w");

    int rows_with_pivot = 0;
    int pivot_counter = 0;
    fread(buffer,sizeof(char),BUFFER_SIZE*BUFFER_SIZE,fp);
    int eof = 0;
    int i = 0;
    while (i< buffer_pow_2 && buffer[i] != '\0')
    {
        int flag = 0;
        while (i < buffer_pow_2 && buffer[i] != '\n' && buffer[i] != '\0')
        {
            if (buffer[i] == pivot)
            {
                // first time pivot char in row
                if (flag == 0)
                {
                    rows_with_pivot++;
                    flag = 1;
                }
                pivot_counter++;
            }
            i++;
        }
        i++;
    }
    
    char str[BUFFER_SIZE];

    printf("appear number: %d, appears in rows %d \n",pivot_counter, rows_with_pivot);
    sprintf(str,"appear number: %d, appears in rows %d \n",pivot_counter, rows_with_pivot);
    fwrite(str,sizeof(char),strlen(str),measurement_p);
    end_clock(str);
    fwrite(str,sizeof(char),strlen(str),measurement_p);
    fclose(fp);
    fclose(measurement_p);
    return 0;
}
