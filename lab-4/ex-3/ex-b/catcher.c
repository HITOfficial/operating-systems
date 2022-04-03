#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <signal.h>
#include "string.h"
#include <unistd.h>

int CATHED_SIGNALS = 0;

void signal_handle(int sig, siginfo_t *info, void *ucontext){
    int sender_pid = info->si_pid;

    if (sig == SIGRTMIN+1 || sig == SIGUSR2){
        printf("sender received: %d signals\n",CATHED_SIGNALS);
        exit(0);
    }
    else {
        kill(sender_pid,SIGUSR1);
        CATHED_SIGNALS ++;
    }
};



int main(int argc, char *argv[]){
    pid_t pid = getpid();

    printf("CATCHER [PID]: %d\n", pid);
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = &signal_handle;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    sigaction(SIGRTMIN,&sa,NULL);
    sigaction(SIGRTMIN+1,&sa,NULL);

    while(1){

        pause();
    }

    return 0;
}