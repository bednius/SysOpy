//
// Created by kreska on 25.04.17.
//

#include "common.h"
#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

char que_name[50];

int c_que_id;

int s_que_id;

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

    s_que_id = connect_to_server();
    msg_buf msg;

    struct mq_attr attr;
    attr.mq_maxmsg = QUE_SIZE;
    attr.mq_msgsize = MSG_SIZE;
    sprintf(que_name, "/que%d", getpid());
    c_que_id = mq_open(que_name, O_CREAT | O_RDONLY, 0600, &attr);

    /*msg.id = msgget(IPC_PRIVATE, 0600);
    c_que_id = msg.id;
    msg.mtype = LOGIN;


    if (msgsnd(s_que_id, &msg, MSG_SIZE, 0)) {
        printf("Cannot login to the server.\n");
        exit(EXIT_FAILURE);
    }*/

    msg.mtype = LOGIN;
    //msg.id = c_que_id;
    strcpy(msg.message, que_name);

    mq_send(s_que_id, (char *) &msg, MSG_SIZE, 0);


    //receive_msg(c_que_id);
    msg_buf msgxd;
    mq_receive(c_que_id, (char *) &msgxd, MSG_SIZE, NULL);
    printf("Received message from %i:\n", c_que_id);
    printf("%s\n\n\n", msg.message);
    msg.id = msgxd.id;



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

    mq_receive(id, (char *) &msg, MSG_SIZE, NULL);
    printf("Received message from %i:\n", id);
    printf("%s\n\n\n", msg.message);
    if (msg.mtype == 5 || msg.mtype == 1) {
        exit(EXIT_SUCCESS);
    }



    /*if (msgrcv(id, &msg, MSG_SIZE, 0, 0) == -1) {
        printf("Cannot receive message.\n");
        exit(EXIT_FAILURE);
    }
    printf("Received message from %i:\n", id);
    printf("%s\n\n\n", msg.message);
    if(msg.mtype == 5 || msg.mtype == 1) {
        exit(EXIT_SUCCESS);
    }*/
}

void send_msg(msg_buf msg, int id) {
    mq_send(id, (char *) &msg, MSG_SIZE, 0);
    printf("Sent message type %i to QID %i\n", (int) msg.mtype, id);
}


int connect_to_server() {
    int result = mq_open(QUE_NAME, O_WRONLY);
    printf("Connected to the server. Queue name:\t%s.\n", QUE_NAME);
    return result;
}

void exit_handler() {
    mq_close(c_que_id);
    mq_close(s_que_id);
    mq_unlink(que_name);
}

void signal_handler(int sig) {
    printf("XD_1\n");
    exit(EXIT_SUCCESS);
}

