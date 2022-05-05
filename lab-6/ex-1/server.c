#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>

#include <pwd.h>
#include <unistd.h>


#define MAX_NO_CLIENTS 10

#define $HOME getpwuid(getuid())->pw_dir

#define PROJ_ID 10


#define MAX_MSG_LENGTH 1024

typedef struct message {
    long type;
    char body[MAX_MSG_LENGTH];
    int sender_id;
    int receiver_id;
    time_t send_time;
} message;

typedef enum msg_type {
    STOP = 1,
    LIST = 2,
    ALL = 3,
    ONE = 4,
    INIT = 5
} msg_type;

const int MESSAGE_SIZE = sizeof(message) - sizeof(long);



const char* RES_FILE_PATH = "logs.txt";

int CLIENT_QUEUES[MAX_NO_CLIENTS] = { 0 };

key_t generate_key(void);
int create_queue(key_t *key);
int listen(int queue_id);
int receive_msg(int queue_id);
void handle_msg(message *msg);
void add_log(message *msg);
void init_client(char* body);
void stop(int client_id);
void to_one(int receiver_id, int sender_id, char* body);
void to_all(int sender_id, char* body);
int list_all_active_clients(int sender_id) ;
struct tm* get_time(void);
int set_sa_handler(int sig_no, int sa_flags, void (*handler)(int));
void exit_handler(void);
void sigint_handler(int sig_no);



int listen(int queue_id) {
    message msg;
    printf("Server is listening...\n");
    while (1) {
        if((msgrcv(queue_id,&msg,sizeof(message) - sizeof(long),0,0)) == -1) {
            perror("SERVER ERROR WITCH: receiving msg");
            exit(1);
        }
        handle_msg(&msg);
    }
}


void handle_msg(message *msg) {
    // adding new handle to logs file
    add_log(msg);

    switch (msg->type) {
        case INIT:
            init_client(msg->body);
            break;
        case STOP:
            stop(msg->sender_id);
            break;
        case ALL:
            to_all(msg->sender_id, msg->body);
            break;
        case ONE:
            to_one(msg->receiver_id, msg->sender_id, msg->body);
            break;
        case LIST:
            list_all_active_clients(msg->sender_id);
            break;
    }
}

struct tm* get_time(void) {
    time_t time_now = time(NULL);
    struct tm *local_time = localtime(&time_now);
    if (!local_time) {
        perror("SERVER ERROR WITCH: local time.\n");
        return NULL;
    }
    return local_time;
}

void add_log(message *msg) {
    FILE *fp = fopen(RES_FILE_PATH, "a");
    if (!fp) {
        perror("SERVER ERROR WITCH: log file.\n");
        return;
    }

    struct tm *local_time = get_time();

    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d [ID]: %d  [MESSAGE]: %s\n",
                local_time->tm_year + 1900,
                local_time->tm_mon + 1,
                local_time->tm_mday,
                local_time->tm_hour,
                local_time->tm_min,
                local_time->tm_sec,
                msg->sender_id,
                msg->body);

    fclose(fp);
}

void init_client(char* body) {
    int id = 0;
    while(id < MAX_NO_CLIENTS ){
        if (!CLIENT_QUEUES[id]) {
            break;
        };
        id ++;
    }

    if (id == MAX_NO_CLIENTS) {
        perror("SERVER ERROR WITCH: reached maximum number of clients");
        return;
    }

    printf("INITIALIZED NEW CLIENT [ID]: %d.\n", id);
    key_t client_key = (key_t) strtol(body, NULL, 10);

    if ((CLIENT_QUEUES[id] = msgget(client_key, 0)) == -1) {
        perror("SERVER ERROR WITCH: client queue identifier.\n");
    }

    message msg = {
            .type = INIT
    };
    sprintf(msg.body, "%d", id);

    if (msgsnd(CLIENT_QUEUES[id], &msg, MESSAGE_SIZE, 0) == -1) {
        perror("SERVER ERROR WITCH: sending response to client.\n");
        return;
    }

}

void stop(int client_id) {
    if (!CLIENT_QUEUES[client_id]) {
        perror("SERVER ERROR WITCH: cannot stop client.\n");
    }
    CLIENT_QUEUES[client_id] = 0;
    printf("STOP message sent to client [ID]: %d.\n", client_id);
}


void to_one(int receiver_id, int sender_id, char* body) {
    if (!CLIENT_QUEUES[receiver_id]) {
        perror("SERVER ERROR WITCH: client not found");
        return;
    }

    message msg = {
            .type = ONE,
            .sender_id = sender_id,
            .receiver_id = receiver_id,
            .send_time = time(NULL)
    };
    strcpy(msg.body, body);

    if (msgsnd(CLIENT_QUEUES[receiver_id], &msg, MESSAGE_SIZE, 0) == -1) {
        perror("SERVER ERROR WITCH: sending message to client.\n");
        return;
    }

    printf("CLIENT message [ID]: %d 2ONE CLIENT [ID] %d sent to .\n", sender_id, receiver_id);
    return;
}

void to_all(int sender_id, char* body) {
    for (int receiver_id = 0; receiver_id < MAX_NO_CLIENTS; receiver_id++) {
        if (CLIENT_QUEUES[receiver_id] && receiver_id != sender_id) {
            to_one(receiver_id, sender_id, body);
        }
    }
    printf("CLIENT message [ID]: %d 2ALL sent.\n", sender_id);
}

int list_all_active_clients(int sender_id) {
    char buffer[MAX_MSG_LENGTH];
    char tmp[32];
    buffer[0] = '\0';

    for (int receiver_id = 0; receiver_id < MAX_NO_CLIENTS; receiver_id++) {
        if (CLIENT_QUEUES[receiver_id]) {
            sprintf(tmp, "%d\n", receiver_id);
            strcat(buffer, tmp);
        }
    }

    message msg = { .type = LIST };
    strcpy(msg.body, buffer);

    if (msgsnd(CLIENT_QUEUES[sender_id], &msg, MESSAGE_SIZE, 0) == -1) {
        perror("Unable to send a message to the receiver client.\n");
        return -1;
    }

    puts("List message was successfully sent.");
    return 0;
}

int set_sa_handler(int sig_no, int sa_flags, void (*handler)(int)) {

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = sa_flags;
    sa.sa_handler = handler;

    if (sigaction(sig_no, &sa, NULL) == -1) {
        perror("Action cannot be set for the signal.\n");
        return -1;
    }

    return 0;
}

void exit_handler(void) {
    message msg = { .type = STOP };
    for (int receiver_id = 0; receiver_id < MAX_NO_CLIENTS; receiver_id++) {
        if (!CLIENT_QUEUES[receiver_id]) {
            continue;
        } else if (msgsnd(CLIENT_QUEUES[receiver_id], &msg, MESSAGE_SIZE, 0) == -1) {
            fprintf(stderr, "Unable to send a STOP message to the '%d' client.\n", receiver_id);
        } else {
            printf("STOP message was successfully sent to the '%d' client.\n", receiver_id);
        }
    }
}

void sigint_handler(int sig_no) {
    printf("REECEIVED SIGNAL: %d. Shutting down the server\n\n", sig_no);
    exit(0);
}

int main(void) {
    if (atexit(exit_handler) == -1) {
        printf("Cannot exit \n");
        return 1;
    }

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigint_handler;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("SERVER ERROR WITH: signals handler.\n");
        return -1;
    }

    // creating key and server queue
    key_t key;
    int queue_id;
    if((key = ftok($HOME, PROJ_ID)) == - 1){
        perror("SERVER ERROR: ftok key");
        return -1;
    }
    if((queue_id = msgget(key, IPC_CREAT | 0777)) == -1){
        perror("SERVER ERROR: server queue");
        return -1;
    }

    printf("Server created [KEY]: %d  \n", key);

    listen(queue_id);
    return 0;
}