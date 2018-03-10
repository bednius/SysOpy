//
// Created by kreska on 25.04.17.
//

#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include "common.h"

int c_que_id;

void exit_handler();

int connect_to_server();

int create_que();

void log_in(msg_buf *msg, int id);

void send_msg(msg_buf msg, int id);

void receive_msg(int id);

void signal_handler(int sig);

int main() {

    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);

    int s_que_id = connect_to_server();
    msg_buf msg;


    msg.id = msgget(IPC_PRIVATE, 0600);
    c_que_id = msg.id;
    msg.mtype = LOGIN;
    if (msgsnd(s_que_id, &msg, MSG_SIZE, 0)) {
        printf("Cannot login to the server.\n");
        exit(EXIT_FAILURE);
    }
    receive_msg(c_que_id);

    atexit(exit_handler);

    printf("What do you want to do?\n");
    printf("ECHO\t\t\t2\n");
    printf("UPPER\t\t\t3\n");
    printf("TIME\t\t\t4\n");
    printf("TERMINATE\t\t5\n\n");

    while (1) {
        int action;
        scanf("%i", &action);
        if (action < 2 || action > 5) {
            printf("Wrong argument.\n");
            continue;
        }
        if (action == 2 || action == 3) {
            printf("Type message.\n");
            scanf("%s", msg.message);
        }
        msg.mtype = action;
        send_msg(msg, s_que_id);
        receive_msg(c_que_id);
    }

}

void receive_msg(int id) {
    msg_buf msg;
    if (msgrcv(id, &msg, MSG_SIZE, 0, 0) == -1) {
        printf("Cannot receive message.\n");
        exit(EXIT_FAILURE);
    }
    printf("Received message from %i:\n", id);
    printf("%s\n\n\n", msg.message);
    if(msg.mtype == 5 || msg.mtype == 1) {
        exit(EXIT_SUCCESS);
    }
}

void send_msg(msg_buf msg, int id) {
    if (msgsnd(id, &msg, MSG_SIZE, 0) != 0) {
        printf("Error while sending message.\n");
        exit(EXIT_FAILURE);
    }
    printf("Sent message type %i to QID %i\n", (int) msg.mtype, id);
}


int connect_to_server() {
    int result = msgget(ftok(PATH, PROJID), 0);
    printf("Connected to the server. QueueID = %i.\n", result);
    return result;
}

void exit_handler() {
    msgctl(c_que_id, IPC_RMID, NULL);
}

void signal_handler(int sig) {
    printf("XD_1\n");
    exit(EXIT_SUCCESS);
}

