//
// Created by kreska on 27.03.17.
//

#define _GNU_SOURCE
#define _XOPEN_SOURCE 700

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

const char *WHITESPACES = " \t\n";

const int MAX = 10;

void print_error(char *msg);

void command(char *record, int memory_limit, int cpu_limit, int line_num);

void variable(char *record, int line_num);

void check_times(struct rusage *usage);

void run_interpreter(char *filename, int memory_limit, int cpu_limit) {
    FILE *file;
    if (!(file = fopen(filename, "r")))
        print_error("Cannot open file.\n");

    char *record;
    const int MAX_SIZE = 100;
    record = malloc(MAX_SIZE);

    int i = 0;

    while (fgets(record, MAX_SIZE, file) != NULL) {
        int record_size = (int) strlen(record);
        if (record_size > 1) {
            if (*record == '#')
                variable(record, ++i);
            else
                command(record, memory_limit, cpu_limit, ++i);
        }
    }
    fclose(file);
}

void variable(char *record, int line_num) {
    record++;
    record = strtok(record, WHITESPACES);
    char *value = strtok(NULL, "\n");
    if (value != NULL) {
        if (setenv(record, value, 1) != 0) {
            printf("Cannot set environment variable %s as value %s. Line: %d\n", record, value, line_num);
            exit(EXIT_FAILURE);
        }
    } else {
        if (unsetenv(record)) {
            printf("Cannot unset environment variable %s. Line: %d\n", record, line_num);
            exit(EXIT_FAILURE);
        }
    }
}

void command(char *record, int memory_limit, int cpu_limit, int line_num) {
    record = strtok(record, WHITESPACES);
    char *argument = strtok(NULL, WHITESPACES);
    char **argv = calloc((MAX + 1), sizeof(char *));
    argv[0] = record;
    for (int i = 1; i < MAX && argument; ++i) {
        argv[i] = argument;
        if (*argument == '$') {
            char *tmp = getenv(argument + 1);
            if (tmp) {
                argv[i] = tmp;
            } else {
                printf("There is no environment variable like %s. Line: %d\n", argument + 1, line_num);
                exit(EXIT_FAILURE);
            }
        }
        argument = strtok(NULL, WHITESPACES);
    }

    int status;

    pid_t pid = fork();

    if (pid == 0) {
        struct rlimit cpu_new;
        struct rlimit mem_new;
        cpu_new.rlim_max = (rlim_t) cpu_limit;
        cpu_new.rlim_cur = (rlim_t) cpu_limit;
        if (setrlimit(RLIMIT_CPU, &cpu_new) != 0)
            printf("Error while setting CPU limit. Line: %d\n", line_num);
        mem_new.rlim_max = (rlim_t) memory_limit * 1024 * 1024;
        mem_new.rlim_cur = (rlim_t) (memory_limit) * 1024 * 1024;
        if (setrlimit(RLIMIT_AS, &mem_new) != 0)
            printf("Error while setting memory limit. Line: %d\n", line_num);

        if (execvp(record, argv) == -1)
            exit(EXIT_FAILURE);
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        struct rusage usage;
        wait3(&status, 0, &usage);
        int x = WIFEXITED(status);
        check_times(&usage);
        if (WIFEXITED(status) == 0) {
            printf("Child did not terminate normally. Line: %d\n", line_num);
            exit(EXIT_FAILURE);
        }
        if (WEXITSTATUS(status) == 1) {
            printf("Error while executing program. Line: %d\n", line_num);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Cannot create process. Line: %d\n", line_num);
        exit(EXIT_FAILURE);
    }
    free(argv);
}

void check_times(struct rusage *usage) {
    printf("U:\t%.6f\t\tS:\t%.6f\n",
           usage->ru_utime.tv_sec + usage->ru_utime.tv_usec / 1e6,
           usage->ru_stime.tv_sec + usage->ru_stime.tv_usec / 1e6);
}

void print_error(char *msg) {
    printf("%s", msg);
    exit(EXIT_FAILURE);
}


