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

    int s_que_id;
    if ((s_que_id = msgget(ftok(PATH, PROJID), IPC_CREAT | IPC_EXCL | 0600)) == -1) {
        if (errno == EEXIST) {
            s_que_id = msgget(ftok(PATH, PROJID), 0);
        }
    }
    queue_id = s_que_id;
    printf("Server ID: %i\n", s_que_id);
    while (1) {
        receive_message(s_que_id);
    }
}

void receive_message(int id) {
    msg_buf msg;
    msgrcv(id, &msg, MSG_SIZE, 0, 0);
    switch (msg.mtype) {
        case LOGIN: {
            if (clients_num < CLIENTS_MAX) {
                clients[clients_num] = msg.id;
                snprintf(msg.message, TEXT_SIZE, "You have been successfully logged in");
                printf("%i you are welcome!\n", msg.id);
                msg.id = clients_num;
                msg.mtype = 3;
                msgsnd(clients[clients_num], &msg, MSG_SIZE, 0);
                ++clients_num;
            } else {
                msg.mtype = 5;
                snprintf(msg.message, TEXT_SIZE, "Achieved max number of clients. Terminating.");
                msgsnd(msg.id, &msg, TEXT_SIZE, 0);
            }
            break;
        }
        case UPPER: {
            to_upper(msg.message);
            msgsnd(msg.id, &msg, TEXT_SIZE, 0);
            break;
        }
        case ECHO: {
            msgsnd(msg.id, &msg, TEXT_SIZE, 0);
            break;
        }
        case TIME: {
            time_t curr_time;
            time(&curr_time);
            sprintf(msg.message, "%s", ctime(&curr_time));
            msgsnd(msg.id, &msg, TEXT_SIZE, 0);
            break;
        }
        case TERMINATE: {
            snprintf(msg.message, TEXT_SIZE, "Terminate.");
            msgsnd(msg.id, &msg, TEXT_SIZE, 0);
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
    msgctl(queue_id, IPC_RMID, NULL);
}

