#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <unistd.h>

void *writer_thread(void *args);

void *reader_thread(void *args);

void exit_handler(void);

void signal_handler(int sig_num);

long get_tid();

#define ARRAY_SIZE 1024

int ver;
int divisor;
sem_t for_print;
int writers_number;
int readers_number;
pthread_t *writers;
pthread_t *readers;
int array[ARRAY_SIZE];
int current_readers = 0;
sem_t for_readers;
sem_t for_writers;

int main(int argc, char **argv) {

    signal(SIGINT, signal_handler);
    atexit(exit_handler);

    if (argc != 4) {
        printf("Wrong arguments. Type 1) version (0 or 1) 2) number of readers 3) number of writers\n");
        return 1;
    }
    ver = atoi(argv[1]);
    readers_number = atoi(argv[2]);
    writers_number = atoi(argv[3]);

    srand((unsigned int) time(NULL));

    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() & 256;
    }

    divisor = rand() % 32;

    writers = malloc(sizeof(pthread_t) * writers_number);
    readers = malloc(sizeof(pthread_t) * readers_number);

    sem_init(&for_print, 0, 1);
    sem_init(&for_readers, 0, 1);
    sem_init(&for_writers, 0, 1);

    for (int i = 0; i < writers_number; ++i) {
        if (pthread_create(&writers[i], NULL, &writer_thread, NULL) != 0) {
            printf("Error while creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < readers_number; ++i) {
        if (pthread_create(&readers[i], NULL, &reader_thread, NULL) != 0) {
            printf("Error while creating thread\n");
            exit(EXIT_FAILURE);
        }
    }


    for (int i = 0; i < writers_number; ++i)
        if (pthread_join(writers[i], NULL) != 0) {
            printf("Error while joining thread\n");
            exit(EXIT_FAILURE);
        }

    for (int i = 0; i < readers_number; ++i)
        if (pthread_join(readers[i], NULL) != 0) {
            printf("Error while joining thread\n");
            exit(EXIT_FAILURE);
        }
    return 0;
}

void exit_handler(void) {
    sem_destroy(&for_print);
    sem_destroy(&for_writers);
    sem_destroy(&for_readers);
    free(writers);
    free(readers);
}

void *reader_thread(void *args) {
    for (;;) {
        sem_wait(&for_readers);
        //current_readers += 1;
        if (++current_readers == 1) {
            sem_wait(&for_writers);
        }
        sem_post(&for_readers);

        int found_numbers = 0;

        for (int i = 0; i < ARRAY_SIZE; ++i) {
            if (array[i] % divisor == 0) {
                if (ver == 1) {
                    sem_wait(&for_print);
                    printf("%ld found number:\t%d\tindex:\t%d\n", get_tid(), array[i], i);
                    fflush(stdout);
                    sem_post(&for_print);
                }
                ++found_numbers;
            }
        }

        sem_wait(&for_print);
        printf("%ld found\t%d\tnumbers\n",get_tid(), found_numbers);
        fflush(stdout);
        sem_post(&for_print);

        sem_wait(&for_readers);
        if (--current_readers == 0) {
            sem_post(&for_writers);
        }
        sem_post(&for_readers);
    }
}

void signal_handler(int sig_num) {
    exit(EXIT_SUCCESS);
}

void *writer_thread(void *args) {
    for (;;) {
        int n = rand() % 128;
        sem_wait(&for_writers);
        for (int i = 0; i < n; ++i) {
            int value = rand() % 256;
            int index = rand() % ARRAY_SIZE;
            array[index] = value;
            if (ver == 1) {
                printf("%ld modified number\tindex:\t%i\tvalue:\t%i\n", get_tid(), index, value);
            } else {
                printf("%ld modified number\n", get_tid());
            }
            fflush(stdout);
        }
        sem_post(&for_writers);
    }
}

long get_tid() {
    return syscall(SYS_gettid);
}
