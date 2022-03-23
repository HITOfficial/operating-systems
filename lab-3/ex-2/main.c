#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int BUFFER_SIZE = 256;

// F(X) = 1/(1+x^2)
double F(double x)
{
    return 4 / (1 + (x * x));
}

// is an Riemann Integral if left side sum is equal to the right side sum of rectangles
void calculate_Riemann_rectangles(int process, double h, int rectangles_per_process, double x)
{

    char res[100] = "w";
    char res_number[10];
    sprintf(res_number, "%d", process);
    strcat(res, res_number);
    strcat(res, ".txt");
    FILE *fp = fopen(res, "w+");
    int r = 1;
    // I had some troubles with for loop with pid (didn't recognize in loop variable), so used while loop
    while (r < rectangles_per_process)
    {
        x += h;
        double f_x = F(x) * h;
        printf("PPID: %d PID: %d  F(x)= %f  \n", getppid(), getpid(), f_x);
        fprintf(fp, "%f \n", f_x);
        r++;
    }
    {
    }
    fclose(fp);
}
int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        printf("to less args");
        return 0;
    }
    clock_t start = clock();

    int a = 0;
    int b = 1;
    // width of single rectangle
    double h = atof(argv[1]);

    // total number of processes
    int n = atoi(argv[2]);

    // total number of rectangles
    double rectangles = (b - a) / h;

    // processes per one rectangle
    int rectangles_per_process = (int)(rectangles / n);

    // using only single process to calculate integral
    if (rectangles_per_process == 0)
    {
        printf("to less processes");
        return 0;
    }

    int id;
    double right_edge;
    for (int process = 1; process <= n; process++)
    {
        id = fork();
        if (id == 0)
        {
            right_edge = process * rectangles_per_process * h;
            calculate_Riemann_rectangles(process, h, rectangles_per_process, right_edge);
            exit(0);
        }
    }
    // waiting till the end of all processes
    while (wait(NULL) != -1)
    {
    }

    double integral_result = 0;
    FILE *fp = fopen("result.txt", "w+");
    for (int process = 1; process <= n; process++)
    {
        char res[11] = "w";
        char res_number[10];
        sprintf(res_number, "%d", process);
        strcat(res, res_number);
        strcat(res, ".txt");
        // reading every single result
        FILE *rp = fopen(res, "r");
        char buffer[BUFFER_SIZE];
        while (!feof(rp))
        {
            fgets(buffer, 256, rp);
            double a = atof(buffer);
            integral_result += a;
        }
        fclose(rp);
    }
    clock_t difference = clock() - start;
    printf("TOTAL TIME: %ld INTEGRAL RESULT: %f", difference, integral_result);
    fprintf(fp, "TOTAL TIME: %ld INTEGRAL RESULT: %f", difference, integral_result);
    fclose(fp);
    return 0;
}
