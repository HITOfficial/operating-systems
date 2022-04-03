#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <signal.h>
#include "string.h"
#include <unistd.h>

int CATHED_SIGNALS = 0;


void signal_handle(int sig, siginfo_t *info, void *ucontext){
//    printf("sender received signal from [PID]: %d [id]:  %d \n", info->si_pid, sig);
    if (sig == SIGRTMIN+1 || sig == SIGUSR2) {
        printf("sender received: %d signals\n",CATHED_SIGNALS);
        exit(0);
    }
    else {
        CATHED_SIGNALS ++;
    }
};


int main(int argc, char *argv[]){
    pid_t pid = getpid();
    pid_t pid_signal = atoi(argv[1]);
    int n = atoi(argv[2]);
    char *type = argv[3];

    printf(" SENDER [PID]: %d\n", pid);
    int i =0;
    if (strcmp( type, "kill")){
        while(i < n){
            kill(pid_signal,SIGUSR1);
            i ++;
        }
        kill(pid_signal,SIGUSR2);

    } else if (strcmp(type, "sigqueue")) {
        while(i < n){
            sigqueue(pid_signal,SIGUSR1,(const union sigval) getpid());
            i ++;
        }
        sigqueue(pid_signal,SIGUSR2,(const union sigval) getpid());

    } else {
        while(i < n){
            kill(pid_signal,SIGRTMIN);
            i ++;
        }
        kill(pid_signal,SIGRTMIN+1);

    }


    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = &signal_handle;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGRTMIN, &sa, NULL);
    sigaction(SIGRTMIN+1, &sa, NULL);



    while (1){
        pause();
    }

}