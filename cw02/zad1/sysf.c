//
// Created by kreska on 19.03.17.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

void sys_sort(const char *filename, long long int record_size, long long int num_of_records) {
    record_size++;  // '\n' char at the end of line
    int file;

    if ((file = open(filename, O_RDWR)) < 0) {
        printf("Cannot open %s file.\n", filename);
        close(file);
        exit(EXIT_FAILURE);
    }

    char *buffer1 = malloc(sizeof(char) * record_size);
    char *buffer2 = malloc(sizeof(char) * record_size);

    bool flag = true;

    while (flag) {
        flag = false;
        lseek(file, 0, SEEK_SET);
        read(file, buffer1, (size_t) record_size);

        for (int i = 1; i < num_of_records; ++i) {
            read(file, buffer2, (size_t) record_size);
            if (buffer1[0] > buffer2[0]) {
                lseek(file, record_size * (-2), SEEK_CUR);
                write(file, buffer2, (size_t) record_size);
                write(file, buffer1, (size_t) record_size);
                flag = true;
            } else {
                char *tmp = buffer2;
                buffer2 = buffer1;
                buffer1 = tmp;
            }
        }
        num_of_records--;
    }
    close(file);
    free(buffer1);
    free(buffer2);
}


void sys_shuffle(const char *filename, long long int record_size, long long int num_of_lines) {
    int file;
    record_size++;

    if ((file = open(filename, O_RDWR)) < 0) {
        printf("Cannot open %s file\n", filename);
        close(file);
        exit(EXIT_FAILURE);
    }

    char *buffer1 = malloc(sizeof(char) * record_size);
    char *buffer2 = malloc(sizeof(char) * record_size);

    for (int i = 0; i < num_of_lines - 1; ++i) {
        int j = (int) (rand() % (num_of_lines - i) + i);
        read(file, buffer1, (size_t) record_size);
        lseek(file, j * record_size, SEEK_SET);
        read(file, buffer2, (size_t) record_size);
        lseek(file, j * record_size, SEEK_SET);
        write(file, buffer1, (size_t) record_size);
        lseek(file, i * record_size, SEEK_SET);
        write(file, buffer2, (size_t) record_size);
    }
    close(file);
    free(buffer1);
    free(buffer2);
}


