//
// Created by kreska on 25.04.17.
//

#include "common.h"
#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>

int clients[CLIENTS_MAX];
int clients_num = 1;
int queue_id;

void receive_message(int id);

void to_upper(char *message);

void signal_handler(int sig);

void exit_handler();

int main() {
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    atexit(exit_handler);


    struct mq_attr attr;
    attr.mq_maxmsg = QUE_SIZE;
    attr.mq_msgsize = MSG_SIZE;
    int s_que_id = mq_open(QUE_NAME, O_CREAT | O_RDONLY, 0600, &attr);

    queue_id = s_que_id;
    printf("Server ID: %i\n", s_que_id);
    while (1) {
        receive_message(s_que_id);
    }
}

void receive_message(int id) {
    msg_buf msg;
    mq_receive(id, (char*)&msg, MSG_SIZE, NULL);
    //printf("%i   %s  %i\n", msg.id, msg.message, msg.mtype);
    switch (msg.mtype) {
        case LOGIN: {
            if (clients_num < CLIENTS_MAX) {
                clients[clients_num] = mq_open(msg.message, O_WRONLY);
                printf("%s %d you are welcome!\n", msg.message, clients[clients_num]);
                snprintf(msg.message, TEXT_SIZE, "You have been successfully logged in");
                msg.id = clients_num;
                msg.mtype = 3;
                mq_send(clients[clients_num], (char *) &msg, MSG_SIZE, 0);
                ++clients_num;
            } else {
                msg.mtype = 5;
                int tmp_id = mq_open(msg.message, O_WRONLY);
                snprintf(msg.message, TEXT_SIZE, "Achieved max number of clients. Terminating.");
                mq_send(tmp_id, (char *) &msg, MSG_SIZE, 0);
                mq_close(tmp_id);
            }
            break;
        }
        case UPPER: {
            to_upper(msg.message);
            mq_send(clients[msg.id], (char *) &msg, MSG_SIZE, 0);
            break;
        }
        case ECHO: {
            mq_send(clients[msg.id], (char *) &msg, MSG_SIZE, 0);
            break;
        }
        case TIME: {
            time_t curr_time;
            time(&curr_time);
            sprintf(msg.message, "%s", ctime(&curr_time));
            mq_send(clients[msg.id], (char *) &msg, MSG_SIZE, 0);
            break;
        }
        case TERMINATE: {
            snprintf(msg.message, TEXT_SIZE, "Terminate.");
            mq_send(clients[msg.id], (char *) &msg, MSG_SIZE, 0);
            break;
        }
        default: {
        }
    }
}

void to_upper(char *message) {
    int i = 0;
    while(message[i]) {
        message[i] = (char) toupper(message[i]);
        ++i;
    }
}

void signal_handler(int sig) {
    printf("XD_1\n");
    exit(EXIT_SUCCESS);
}

void exit_handler(void) {
    printf("XD_2\n");
    for (int i = 1; i < clients_num; ++i) {
        mq_close(clients[i]);
    }
    mq_close(queue_id);
    mq_unlink(QUE_NAME);
}

