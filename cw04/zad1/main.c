#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

bool flag = true;

void sighandler(int sig_no);

int main() {
    signal(SIGINT, &sighandler);
    struct sigaction action;
    action.sa_handler = sighandler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGTSTP, &action, NULL);
    int i = -1;
    while (true) {
        if (flag)
            ++i;
        else
            --i;
        i %= 26;
        printf("%c ", abs(26 + i) % 26 + 'A');
        fflush(stdout);
        usleep(300000);
    }
}

void sighandler(int sig_no) {
    if (sig_no == SIGTSTP) {
        flag = !flag;
        printf("(reverse) ");
    } else if (sig_no == SIGINT) {
        printf("\nReceived signal SIGINT\n");
        exit(EXIT_SUCCESS);
    }
}