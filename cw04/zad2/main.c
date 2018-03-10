#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


int K;
int PARENTID;
int *childrens;
int RECEIVED = 0;

void make_childs(int N);

void receive_signal(int sig_num, siginfo_t *sig_info, void *sig_context);

void child_handler(int sig_num);

void receive_real_signal(int sig_num, siginfo_t *sig_info, void *sig_context);

void handle_int(int sig_num);


int main(int argc, char **argv) {
    RECEIVED = 0;
    int n = atoi(argv[1]);
    childrens = calloc(n, sizeof(int));
    K = atoi(argv[2]);
    make_childs(n);
    free(childrens);
    printf("Ende!\n");
    sleep(2);
    return 0;
}

void make_childs(int N) {

    sigset_t block_mask;
    sigfillset(&block_mask);

    struct sigaction action;
    memset(&action, '\0', sizeof(action));

    action.sa_handler = &handle_int;
    action.sa_mask = block_mask;
    sigaction(SIGINT, &action, NULL);
    action.sa_handler = NULL;

    sigdelset(&block_mask, SIGINT);
    action.sa_sigaction = &receive_signal;
    action.sa_flags = SA_SIGINFO | SA_RESTART;
    action.sa_mask = block_mask;

    sigaction(SIGUSR1, &action, NULL);

    action.sa_sigaction = &receive_real_signal;

    for (int i = SIGRTMIN; i <= SIGRTMAX; ++i) {
        sigaction(i, &action, NULL);
    }

    PARENTID = getpid();
    printf("Mother PID: %d\n", getpid());
    for (int i = 0; i < N; ++i) {
        pid_t pid = fork();
        switch (pid) {
            case -1: {
                printf("Error while creating child process.\n");
                pid_t wpid;
                while ((wpid = wait(NULL)) > 0) {
                    kill(wpid, SIGKILL);
                }
                exit(EXIT_FAILURE);
            }
            case 0: {
                srand(time(NULL) ^ (getpid() << 8));
                action.sa_flags = SA_RESTART;
                action.sa_handler = &child_handler;
                sigaction(SIGUSR2, &action, NULL);
                //signal(SIGUSR2, &child_handler);
                printf("Child %d, PID: %d\n", i + 1, getpid());
                sleep(rand() % 9 + 2);
                time_t start = time(NULL);
                kill(PARENTID, SIGUSR1);
                sleep(10);
                kill(PARENTID, rand() % (SIGRTMAX - SIGRTMIN + 1) + SIGRTMIN);
                time_t act = time(NULL) - start;
                printf("PID: %d, time: %.6f\n", getpid(), (double) (act));
                free(childrens);
                exit((int) act);
            }
            default: {
                usleep(100);
            }
        }
    }
    while (wait(NULL) > 0) {}
}

void receive_signal(int sig_num, siginfo_t *sig_info, void *sig_context) {
    printf("%d asked for permission\n", sig_info->si_pid);
    childrens[RECEIVED++] = sig_info->si_pid;
    if (RECEIVED == K) {
        for (int i = 0; i < K; ++i) {
            kill(childrens[i], SIGUSR2);
        }
    } else if (RECEIVED > K) {
        kill(sig_info->si_pid, SIGUSR2);
    }
}

void child_handler(int sig_num) {
    //for unpause children
}

void receive_real_signal(int sig_num, siginfo_t *sig_info, void *sig_context) {
    printf("Received signal %d from %d\n", sig_num, sig_info->si_pid);
}

void handle_int(int sig_num) {
    free(childrens);
    kill(getpid(), SIGKILL);
}