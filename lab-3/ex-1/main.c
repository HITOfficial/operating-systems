#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("pass argument");
        return 0;
    }
    int child_proc_counter = atoi(argv[1]);

    // fork() == 0, when is subproces of parent process
    int id;
    for (int i = 0; i < child_proc_counter; i++)
    {
        id = fork();
        if (id == 0)
        {
            printf("parent: %d child: %d \n", getppid(), getpid());
            // exiting to not continue sub process
            exit(0);
        }
    }

    // waiting till the ending of subprocesses
    while (wait(NULL) != -1)
    {
    }
    return 0;
}
