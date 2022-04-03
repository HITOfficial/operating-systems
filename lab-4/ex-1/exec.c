#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void pending_signal(){
    sigset_t signals_set;
    sigpending(&signals_set);
    if(sigismember(&signals_set, SIGUSR1)) {
        printf("[PPID] %d [PID] %d PENDING SIGNAL ON EXEC AND IS A PART OF PROC\n",getppid(), getpid());
    } else {
        printf("[PPID] %d [PID] %d PENDING SIGNAL ON EXEC AND IS NOT A PART OF PROC\n",getppid(), getpid());
    }
}

int main(int argc, char *argv[]) {
    printf("executing proc \n");
    raise(SIGUSR1);
    pending_signal();
    printf("ending of child process \n");
    return 0;
}