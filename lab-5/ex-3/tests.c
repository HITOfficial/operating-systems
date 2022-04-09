#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    mkfifo("/tmp/myfifo", S_IRUSR | S_IWUSR); // create named pipe

    if (fork() == 0) {
        execlp("./producer", "./producer", "/tmp/myfifo", "first.txt", "0", "10", NULL);
    }

    if (fork() == 0) {
        execlp("./producer", "./producer", "/tmp/myfifo", "second.txt", "1", "10", NULL);
    }

    if (fork() == 0) {
        execlp("./producer", "./producer", "/tmp/myfifo", "third.txt", "2", "10", NULL);
    }

    if (fork() == 0) {
        execlp("./producer", "./producer", "/tmp/myfifo", "fourth.txt", "3", "10", NULL);
    }

    if (fork() == 0) {
        execlp("./producer", "./producer", "/tmp/myfifo", "fifth.txt", "4", "10", NULL);
    }

    if (fork() == 0) {
        execlp("./consumer", "./consumer", "/tmp/myfifo", "result.txt", "10", NULL);
    }


    while(wait(NULL) > -1){
    }

}