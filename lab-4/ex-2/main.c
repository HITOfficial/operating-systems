#include <stdio.h>
#include <signal.h>
#include <string.h>

void siginfo_handler(int sig, siginfo_t *info, void *ucontext){
    printf("received [SA_SIGINFO] signal NR: %d, [PID]: %d \n", sig, info->si_pid);
}

void resethand_handler(int sig){
    printf("received [SA_RESETHAND] signal NR: %d\n", sig);
}

void oneshot_handler(int sig){
    printf("received [SA_ONSTACK] signal NR: %d\n", sig);
}


int main(int argc, char *argv[]){

    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = &siginfo_handler;
    sigaction(SIGUSR1,&sa,NULL);
    raise(SIGUSR1);

    sigemptyset(&sa.sa_mask);
    sa.sa_handler = &oneshot_handler;
    sa.sa_flags = SA_ONESHOT;
    sigaction(SIGUSR2,&sa,NULL);
    raise(SIGUSR2);

    sa.sa_handler = &resethand_handler;
    sa.sa_flags = SA_RESETHAND;
    sigaction(SIGUSR1,&sa,NULL);
    raise(SIGUSR1);

    return 0;
}