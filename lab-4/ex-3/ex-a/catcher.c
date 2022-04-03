#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <signal.h>
#include "string.h"
#include <unistd.h>

int CATHED_SIGNALS = 0;
int SENDER_PID;

void sending_back_signals(int pid, int sig) {
    printf("catcher received: %d signals, and starting to send it back to sender\n", CATHED_SIGNALS);
    for (int i =0; i< CATHED_SIGNALS; i++) {
        if (sig == SIGUSR1) {
            kill(pid,SIGUSR1);
        }
        else {
            kill(pid, SIGRTMIN);
        }
    }
    if (sig == SIGUSR2) {
        kill(pid,SIGUSR2);
    }
    else {
        kill(pid,SIGRTMIN+1);
    }

    exit(0);
}

void signal_handle(int sig, siginfo_t *info, void *ucontext){
//    printf("catcher received signal from [PID]: %d [id]: %d \n", info->si_pid, sig);
    if (sig == SIGRTMIN+1 || sig == SIGUSR2){
        SENDER_PID = info->si_pid;
        sending_back_signals(SENDER_PID, sig);
    }
    else {
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