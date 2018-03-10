//
// Created by kreska on 25.04.17.
//

#ifndef INC_6RANO_COMMON_H
#define INC_6RANO_COMMON_H

#include <stdlib.h>

#define CLIENTS_MAX 8
#define TEXT_SIZE 128
#define MSG_SIZE (sizeof(struct msg_buf) - sizeof(long))
#define PATH getenv("PATH")
#define PROJID 5


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
