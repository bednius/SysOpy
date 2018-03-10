#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


int L;
pid_t ppid;
pid_t cpid;
int child_counter = 0;
int parent_counter = 0;
int send_counter = 0;

void int_handler(int sig_num);

void child_handler(int sig_num);

void child_handler2(int sig_num);

void end_handler(int sig_num);

void parent_handler(int sig_num);

int main(int argc, char **argv) {
    int type = atoi(argv[2]);
    L = atoi(argv[1]);
    ppid = getpid();

    srand(time(NULL));
    int SIGNAL = SIGRTMIN + (rand() % (SIGRTMAX - SIGRTMIN));
    int SIGNAL_END = SIGRTMIN + (rand() % (SIGRTMAX - SIGRTMIN));

    printf("Mother PID: %d\n", ppid);

    sigset_t block;
    sigfillset(&block);
    struct sigaction action;
    memset(&action, '\0', sizeof(action));
    action.sa_mask = block;
    action.sa_handler = int_handler;
    sigaction(SIGINT, &action, NULL);

    struct sigaction action1;
    memset(&action1, '\0', sizeof(action1));
    action1.sa_mask = block;
    action1.sa_handler = parent_handler;
    action1.sa_flags = SA_RESTART;
    sigaction(SIGRTMAX, &action1, NULL);
    sigaction(SIGRTMIN, &action1, NULL);
    signal(SIGUSR1, parent_handler);

    cpid = fork();
    sleep(1);
    switch (cpid) {
        case -1: {
            printf("Fork error\n");
            exit(EXIT_FAILURE);
        }
        case 0: {
            signal(SIGUSR1, child_handler);
            signal(SIGUSR2, end_handler);

            sigset_t real_mask;
            sigfillset(&real_mask);
            struct sigaction real_action;
            memset(&real_action, '\0', sizeof(action));
            real_action.sa_mask = real_mask;
            real_action.sa_flags = SA_RESTART;
            real_action.sa_handler = end_handler;
            sigaction(SIGRTMAX, &real_action, NULL);
            real_action.sa_handler = child_handler2;
            sigaction(SIGRTMIN, &real_action, NULL);
            while (1);
        }
        default : {
            sleep(1);
            switch (type) {
                case 1 : {
                    for (int i = 0; i < L; ++i) {
                        kill(cpid, SIGUSR1);
                    }
                    sleep(1);
                    kill(cpid, SIGUSR2);
                    break;
                }
                case 2 : {
                    union sigval val;
                    memset(&val, '\0', sizeof(val));
                    for (int i = 0; i < L; ++i) {
                        sigqueue(cpid, SIGUSR1, val);
                    }
                    sleep(1);
                    sigqueue(cpid, SIGUSR2, val);
                    break;
                }
                case 3 : {
                    for (int i = 0; i < L; ++i) {
                        kill(cpid, SIGRTMIN);
                    }
                    sleep(1);
                    kill(cpid, SIGRTMAX);
                    break;
                }
                default: {
                    printf("Wrong Type\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    wait(NULL);
    printf("Total number of signals received by parent process:\t %d\n", parent_counter);
    return 0;
}

void int_handler(int sig_num) {
    kill(cpid, SIGKILL);
    kill(getpid(), SIGKILL);
}

void child_handler(int sig_num) {
    printf("PID: %d received signal %d from %d\n", getpid(), sig_num, ppid);
    kill(ppid, SIGUSR1);
    ++child_counter;
}

void child_handler2(int sig_num) {
    printf("PID: %d received signal %d from %d\n", getpid(), sig_num, ppid);
    kill(ppid, SIGRTMIN);
    ++child_counter;
}

void end_handler(int sig_num) {
    printf("Total number of signals received by child process:\t %d\n", child_counter);
    printf("Total number of signals send by child process:\t\t %d\n", child_counter);

    exit(EXIT_SUCCESS);
}

void parent_handler(int sig_num) {
    printf("PID: %d received signal %d from %d\n", ppid, sig_num, cpid);
    ++parent_counter;
}