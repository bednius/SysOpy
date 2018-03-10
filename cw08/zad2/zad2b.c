#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_fun(void *args) {
    while (1) {
        usleep(200);
        printf("working\n");
    }
}

void *zero(void *args) {
    int a = 1, b = 0, result;
    while (1) {
        printf("0 divide");
        a /= b;
    }
}

int main() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread_fun, NULL);
    sleep(3);
    pthread_create(&thread2, NULL, zero, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}
