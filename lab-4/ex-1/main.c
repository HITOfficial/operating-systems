#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>



void signal_handler(int id){
    printf("Handling signal: %d \n", id);
}


void pending_signal(){
    sigset_t signals_set;
    sigpending(&signals_set);
    if(sigismember(&signals_set, SIGUSR1)) {
        printf("[PPID] %d [PID] %d PENDING SIGNAL IS A PART OF PROC\n",getppid(), getpid());
    } else {
        printf("[PPID] %d [PID] %d PENDING SIGNAL IS NOT A PART OF PROC\n",getppid(), getpid());
    }
}

enum type {
    IGNORE,
    MASK,
    PENDING,
    HANDLER
};


int main(int argc, char *argv[]){

    if (argc < 2) {
        printf("to less args!");
        return 1;
    }
    char *pivot = argv[1];

    char types[4][10] = {"ignore","handler","mask","pending"};
    int flag = 0;
    for (int i=0;i < 4; i++) {
        if(strcmp(pivot,types[i]) == 0){
            flag = 1;
            break;
        }
    }
    if (!flag) {
        printf("passed bad argument: available: ignore|handler|mask|pending");
        return 1;
    }

    printf("starting main process \n");

    enum type signal_type;
    if(strcmp(pivot,"ignore") == 0) {
        signal_type = IGNORE;
        signal(SIGUSR1,SIG_IGN);
    } else if (strcmp(pivot,"handler") == 0){
        signal_type = HANDLER;
        signal(SIGUSR1,signal_handler);
    } else {
        sigset_t sig_to_block;
        sigemptyset(&sig_to_block);
        sigaddset(&sig_to_block, SIGUSR1);
        sigprocmask(SIG_BLOCK, &sig_to_block, NULL);
    }

    // sending signal
    raise(SIGUSR1);

    pending_signal();

    int id = fork();

    if (id ==0) {
        printf("start of child process \n");
        if (signal_type == PENDING){
            // pending again signal
            pending_signal();
        } else {
            // sending again signal
            raise(SIGUSR1);
        }

        if (signal_type == IGNORE || signal_type == MASK || signal_type == PENDING){
            if (execl("./exec","exec",NULL) == -1){
                printf("cannot execute file ERROR: %s \n",strerror(errno));
            }
        }
        printf("ending of child process \n");
        exit(1);
    }

    wait(NULL);
    printf("ending main process \n");
    return  0;

}


