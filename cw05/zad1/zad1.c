#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include<fcntl.h>
#include <sys/wait.h>

const int MAX = 100;
#define SEPARATORS "|\n"

typedef struct Argv {
    char ***args;
    int i;
} Argv;

Argv parse_line(char *line);

int main() {
    size_t bytes = 0;
    char *line = NULL;
    pid_t pid;
    //char ***argv;

    while (getline(&line, &bytes, stdin) > 1) {
        Argv argv = parse_line(line);
        char ***t = argv.args;

        int fds[argv.i][2];
        for (int i = 0; i < argv.i; ++i)
            pipe(fds[i]);

        if (argv.i == 1) {
            pid = fork();
            switch (pid) {
                case -1: {
                    printf("Error while fork\n");
                    exit(EXIT_FAILURE);
                }
                case 0: {
                    if (execvp(t[0][0], t[0]) == -1) {
                        printf("Exec error\n");
                        exit(EXIT_FAILURE);
                    }
                }
                default: {
                }
            }
        } else {
            pid = fork();
            switch (pid) {
                case -1: {
                    printf("Error while fork\n");
                    exit(EXIT_FAILURE);
                }
                case 0: {
                    if (argv.i > 1) {
                        close(fds[0][0]);
                        dup2(fds[0][1], STDOUT_FILENO);
                    }
                    if (execvp(t[0][0], t[0]) == -1) {
                        printf("Exec error\n");
                        exit(EXIT_FAILURE);
                    }
                }
                default: {
                }
            }

            for (int i = 1; i < argv.i - 1; ++i) {
                pid = fork();

                switch (pid) {
                    case -1 : {
                        printf("Error while fork\n");
                        exit(EXIT_FAILURE);
                    }
                    case 0 : {
                        close(fds[i - 1][1]);
                        dup2(fds[i - 1][0], STDIN_FILENO);

                        close(fds[i][0]);
                        dup2(fds[i][1], STDOUT_FILENO);

                        if (execvp(t[i][0], t[i]) == -1) {
                            printf("Exec error\n");
                            exit(EXIT_FAILURE);
                        }
                    }
                    default : {
                        close(fds[i - 1][0]);
                        close(fds[i - 1][1]);
                    }
                }
            }

            pid = fork();
            switch (pid) {
                case -1: {
                    printf("Error while fork\n");
                    exit(EXIT_FAILURE);
                }
                case 0: {
                    close(fds[argv.i - 2][1]);
                    dup2(fds[argv.i - 2][0], STDIN_FILENO);
                    execvp(t[argv.i - 1][0], t[argv.i - 1]);
                }
                default: {
                    close(fds[argv.i - 2][0]);
                    close(fds[argv.i - 2][1]);
                }
            }
        }
        while (wait(NULL) > 0) {}
    }
    return 0;
}

Argv parse_line(char *line) {
    int i = 0;
    char *commands[MAX];
    commands[0] = strtok(line, SEPARATORS);
    while ((commands[++i] = strtok(NULL, SEPARATORS)));

    char ***result;
    result = calloc(MAX, sizeof(char *));
    for (int j = 0; j < MAX; ++j)
        result[j] = calloc(MAX, sizeof(char *));
    for (int j = 0; j < MAX; ++j)
        for (int k = 0; k < MAX; ++k)
            result[j][k] = calloc(MAX, sizeof(char));

    for (int j = 0; j < i; ++j) {
        int k = 0;
        result[j][k] = strtok(commands[j], " ");
        while ((result[j][++k] = strtok(NULL, " ")) != NULL);
        result[j][k + 1] = NULL;
    }
    result[i] = NULL;
    Argv argv;
    argv.args = result;
    argv.i = i;
    return argv;
}

