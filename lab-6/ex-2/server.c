#include "vals.h"
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define MAX_NO_CLIENTS 100

const char* RES_FILE_PATH = "logs.txt";

int SERVER_QUEUE_FD = -1;
int CLIENT_QUEUES[MAX_NO_CLIENTS] = { 0 };

static long parse_long(char* msg_str);
static int parse_string(char* target, char* msg_str);
void exit_handler(void);
int set_sa_handler(int sig_no, int sa_flags, void (*handler)(int));
void sigint_handler(int sig_no);
int listen(int queue_fd);
int handle_msg(char* msg_str);
struct tm* get_time(void);
int get_free_id(void);
int save_msg(msg_buf *msg);
int init_client(char* name);
int stop_client(int client_id);
int send_to_one(int receiver_id, int sender_id, char* body);
int send_to_all(int sender_id, char* body);
int list_all_active_clients(int sender_id);


msg_buf *parse_msg(char* msg_str) {
    char msg_cp[MAX_MSG_TOTAL_LENGTH];
    strcpy(msg_cp, msg_str);
    // <type>-<body>-<sender_id>-<receiver_id>-<send_time>
    msg_buf *msg = calloc(1, sizeof(msg_buf));
    if ((msg->type = (int) parse_long(msg_cp)) == -1
        || parse_string(msg->body, msg_cp) == -1
        || (msg->sender_id = (int) parse_long(msg_cp)) == -1
        || (msg->receiver_id = (int) parse_long(msg_cp)) == -1
        || (msg->send_time = parse_long(msg_cp)) == -1) {
        fprintf(stderr, "Something went wrong while parsing a message string");
        free(msg);
        return NULL;
    }
    return msg;
}

void create_msg_str(msg_buf msg, char* buff) {
    char temp[MAX_MSG_TOTAL_LENGTH];
    sprintf(temp, "%d%s%s%s%d%s%d%s%ld",
            msg.type,
            MSG_SEP,
            strlen(msg.body) ? msg.body : NULL,
            MSG_SEP,
            msg.sender_id,
            MSG_SEP,
            msg.receiver_id,
            MSG_SEP,
            msg.send_time
    );

    strcpy(buff, temp);
    buff[strlen(buff)] = '\0';
}


int send_msg(int queue_fd, msg_buf msg_obj, int type, char* error_msg) {
    char msg[MAX_MSG_TOTAL_LENGTH];
    create_msg_str(msg_obj, msg);

    if (mq_send(queue_fd, msg, strlen(msg), type) == -1) {
        perror(error_msg);
        return -1;
    }
    return 0;
}

static long parse_long(char* msg_str) {
    char* rest;
    char* num_str;
    if (!(num_str = strtok_r(msg_str, MSG_SEP, &rest))) {
        fprintf(stderr, "Unable to parse long. There are no more tokens in a string.\n");
        return -1;
    }

    long num = strtol(num_str, NULL, 10);
    strcpy(msg_str, rest);
    if (num == 0 && errno) {
        perror("Unable to convert string to the number.\n");
        return -1;
    }

    return num;
}

static int parse_string(char* target, char* msg_str) {
    char* rest;
    char* body;
    if (!(body = strtok_r(msg_str, MSG_SEP, &rest))) {
        fprintf(stderr, "Unable to parse string. There are no more tokens in a string.\n");
        return -1;
    }
    strcpy(target, body);
    strcpy(msg_str, rest);
    return 0;
}


void exit_handler(void) {
    msg_buf msg_obj = { .type = STOP };
    char msg[MAX_MSG_TOTAL_LENGTH];
    create_msg_str(msg_obj, msg);

    for (int receiver_id = 0; receiver_id < MAX_NO_CLIENTS; receiver_id++) {
        if (!CLIENT_QUEUES[receiver_id]) {
            continue;
        } else if (mq_send(CLIENT_QUEUES[receiver_id], msg, strlen(msg), STOP) == -1) {
            fprintf(stderr, "Unable to send a STOP message to the '%d' client.\n", receiver_id);
        } else {
            printf("STOP message was sent to the '%d' client.\n", receiver_id);
        }
    }

    if (mq_close(SERVER_QUEUE_FD) == -1 || mq_unlink(SERVER_QUEUE_PATH)) {
        perror("Unable to remove a queue.\n");
        exit(1);
    }
}

int set_sa_handler(int sig_no, int sa_flags, void (*handler)(int)) {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = sa_flags;
    sa.sa_handler = handler;

    if (sigaction(sig_no, &sa, NULL) == -1) {
        perror("Cannot set action.\n");
        return -1;
    }
    return 0;
}

void sigint_handler(int sig_no) {
    printf("Closing the server...\n", sig_no);
    exit(0);
}

int listen(int queue_fd) {
    printf("listening ...\n");
    char msg[MAX_MSG_TOTAL_LENGTH];
    while (1) {
        if ((mq_receive(queue_fd, msg, MAX_MSG_TOTAL_LENGTH, NULL)) == -1) {
            perror("SERVER ERROR: listening message\n");
            exit(1);
        } else {
            handle_msg(msg);
        }
    }
}


int handle_msg(char* msg_str) {
    msg_buf *msg = parse_msg(msg_str);
    if (save_msg(msg) == -1) return -1;
    int status;

    switch (msg->type) {
        case INIT:
            printf("Received INIT from %d client.\n", msg->sender_id);
            status = init_client(msg->body);
            break;
        case STOP:
            printf("Received STOP from %d client.\n", msg->sender_id);
            status = stop_client(msg->sender_id);
            break;
        case ALL:
            printf("Received 2ALL from %d client.\n", msg->sender_id);
            status = send_to_all(msg->sender_id, msg->body);
            break;
        case ONE:
            printf("Received 2ONE from %d client.\n", msg->sender_id);
            status = send_to_one(msg->receiver_id, msg->sender_id, msg->body);
            break;
        case LIST:
            printf("Received LIST from %d client.\n", msg->sender_id);
            status = list_all_active_clients(msg->sender_id);
            break;
        default:
            fprintf(stderr, "Unrecognized message type '%d'.\n", msg->type);
            status = -1;
    }

    free(msg);
    return status;
}

int save_msg(msg_buf *msg) {
    FILE *f_ptr = fopen(RES_FILE_PATH, "a");
    if (!f_ptr) {
        perror("Failed to open a results file.\n");
        return -1;
    }

    struct tm *local_time = get_time();

    if (fprintf(f_ptr, "%d-%02d-%02d %02d:%02d:%02d\nClient id: %d\nMessage type: %d\nMessage body:\n'%s'\n\n",
                local_time->tm_year + 1900,
                local_time->tm_mon + 1,
                local_time->tm_mday,
                local_time->tm_hour,
                local_time->tm_min,
                local_time->tm_sec,
                msg->sender_id,
                msg->type,
                msg->body
    ) < 0) {
        fprintf(stderr, "Unable to write data to a file.\n");
        return -1;
    }

    fclose(f_ptr);

    return 0;
}

int get_free_id(void) {
    for (int i = 0; i < MAX_NO_CLIENTS; i++) {
        if (!CLIENT_QUEUES[i]) return i;
    }
    return -1;
}

struct tm* get_time(void) {
    time_t time_now = time(NULL);
    struct tm *local_time = localtime(&time_now);
    if (!local_time) {
        perror("Unable to get a local time.\n");
        return NULL;
    }
    return local_time;
}

int init_client(char* name) {
    int id = get_free_id();
    if (id == -1) {
        fprintf(stderr, "Impossible to initialize a new client. The number of possible clients (%d) was exceeded.\n", MAX_NO_CLIENTS);
        return -1;
    }
    printf("NED CLIENT [ID]: %d.\n", id);

    // Add something to ensure that the client id will fit in this array
    if (!(CLIENT_QUEUES[id] = mq_open(name, O_WRONLY))) {
        perror("Unable to create a client queue.\n");
        return -1;
    }

    msg_buf msg = { .type = INIT };
    sprintf(msg.body, "%d", id);
    return send_msg(CLIENT_QUEUES[id], msg, INIT, "Unable to send INIT response to the client.\n");
}

int stop_client(int client_id) {
    if (!CLIENT_QUEUES[client_id]) {
        fprintf(stderr, "Unable to stop a client with id '%d'. Client doesn't exist.\n", client_id);
        return -1;
    }
    if (mq_close(CLIENT_QUEUES[client_id]) == -1) {
        perror("Unable to close client queue.\n");
        return -1;
    }
    CLIENT_QUEUES[client_id] = 0;
    return 0;
}

int send_to_one(int receiver_id, int sender_id, char* body) {
    if (!CLIENT_QUEUES[receiver_id]) {
        fprintf(stderr, "Unable to send a message to a client. There is no client with id '%d'.\n", receiver_id);
        return -1;
    }
    msg_buf msg = {
            .type = ONE,
            .sender_id = sender_id,
            .receiver_id = receiver_id,
            .send_time = time(NULL)
    };
    strcpy(msg.body, body);

    if (send_msg(CLIENT_QUEUES[receiver_id], msg, ONE, "Cannot send message to receiver.\n") == -1) {
        return -1;
    } else {
        printf("Message from '%d' to '%d' was successfully sent.\n", sender_id, receiver_id);
        return 0;
    }
}

int send_to_all(int sender_id, char* body) {
    for (int receiver_id = 0; receiver_id < MAX_NO_CLIENTS; receiver_id++) {
        if (CLIENT_QUEUES[receiver_id] && receiver_id != sender_id) {
            if (send_to_one(receiver_id, sender_id, body) == -1) return -1;
        }
    }

    printf("Message from '%d' was successfully sent to all remaining clients.\n", sender_id);
    return 0;
}

int list_all_active_clients(int sender_id) {
    printf("All active clients:\n");
    char buff[MAX_BODY_LENGTH];
    char temp[32];
    buff[0] = '\0';

    for (int receiver_id = 0; receiver_id < MAX_NO_CLIENTS; receiver_id++) {
        if (CLIENT_QUEUES[receiver_id]) {
            printf("%d\n", receiver_id);
            sprintf(temp, "%d\n", receiver_id);
            strcat(buff, temp);
        }
    }

    msg_buf msg = { .type = LIST };
    strcpy(msg.body, buff);

    if (send_msg(CLIENT_QUEUES[sender_id], msg, LIST, "Unable to send a message to the receiver client.\n") == -1) {
        return -1;
    }
    puts("List message was successfully sent.");
    return 0;
}


int main(void) {
    if (atexit(exit_handler) == -1) {
        return 1;
    }
    if (set_sa_handler(SIGINT, 0, sigint_handler) == -1) return 2;


    struct mq_attr attr = {
            .mq_msgsize = MAX_MSG_TOTAL_LENGTH,
            .mq_maxmsg = MAX_SERVER_MSG_COUNT
    };
    SERVER_QUEUE_FD = mq_open(SERVER_QUEUE_PATH, O_RDONLY | O_CREAT, 0666, &attr);
    // cannot open server queue
    if (SERVER_QUEUE_FD == -1) return 3;

    puts("Created SERVER QUEUE");

    listen(SERVER_QUEUE_FD);
    return 4;
}