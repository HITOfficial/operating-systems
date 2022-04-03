#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <signal.h>
#include "string.h"
#include <unistd.h>

int CATHED_SIGNALS = 0;
int SENDED_SINGALS = 0;
int SIGNALS_TO_SEND;
char * TYPE;
pid_t PID_SIGNAL;


void signal_handle(int sig, siginfo_t *info, void *ucontext){
//    printf("sender received signal from [PID]: %d [id]:  %d \n", info->si_pid, sig);
    CATHED_SIGNALS ++;
    if (SENDED_SINGALS < SIGNALS_TO_SEND) {
        if (strcmp( TYPE, "kill")==0){
            kill(PID_SIGNAL,SIGUSR1);
        } else if (strcmp(TYPE, "sigqueue")==0) {
            sigqueue(PID_SIGNAL,SIGUSR1,(const union sigval) getpid());
        } else {
            kill(PID_SIGNAL,SIGRTMIN);
        }
        SENDED_SINGALS ++;
    }
    // sending last signal to end processes
    else if (SENDED_SINGALS == SIGNALS_TO_SEND) {
        if (strcmp( TYPE, "kill") == 0){
            kill(PID_SIGNAL, SIGUSR2);
        } else if (strcmp( TYPE, "sigqueue")== 0) {
            sigqueue(PID_SIGNAL,SIGUSR2,(const union sigval) getpid());
        }
        else {
            kill(PID_SIGNAL,SIGRTMIN+1);
        }
        printf("sender received: %d signals\n",CATHED_SIGNALS);
        exit(0);
    }
}


int main(int argc, char *argv[]){
    pid_t pid = getpid();
    pid_t PID_SIGNAL = atoi(argv[1]);
    SIGNALS_TO_SEND = atoi(argv[2]);
    TYPE = argv[3];

    printf(" SENDER [PID]: %d\n", pid);

    if (strcmp( TYPE, "kill")== 0){
        kill(PID_SIGNAL,SIGUSR1);
        SENDED_SINGALS ++;

    } else if (strcmp(TYPE, "sigqueue")== 0) {
        sigqueue(PID_SIGNAL,SIGUSR1,(const union sigval) getpid());
        SENDED_SINGALS ++;
    } else {
        kill(PID_SIGNAL,SIGRTMIN);
        SENDED_SINGALS ++;
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