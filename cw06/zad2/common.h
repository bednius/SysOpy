//
// Created by kreska on 25.04.17.
//

#ifndef INC_6RANO_COMMON_H
#define INC_6RANO_COMMON_H

#include <stdlib.h>
#include <mqueue.h>

#define CLIENTS_MAX 8
#define TEXT_SIZE 128
#define MSG_SIZE sizeof(struct msg_buf)
#define QUE_SIZE 10
#define QUE_NAME "/quebonafide"


typedef struct msg_buf {
    long mtype;
    int id;
    char message[TEXT_SIZE];

} msg_buf;

enum msg_type {
    LOGIN = 1,
    ECHO,
    UPPER,
    TIME,
    TERMINATE
};

#endif //INC_6RANO_COMMON_H
