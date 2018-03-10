#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

FILE *file;
char *pattern;
int records_number;
int threads_number;
pthread_t *thread_ids;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool found = false;

void safe_open(char *filename);

void *work(void *arg);

int main(int argc, char **argv) {
    if (argc != 5) {
        printf("Type 4 args:\n1) number of threads\n2) filename\n3) number of records\n4) word pattern\n");
        exit(EXIT_FAILURE);
    }

    threads_number = atoi(argv[1]);
    char *filename = argv[2];
    records_number = atoi(argv[3]);
    pattern = argv[4];

    safe_open(filename);

    thread_ids = calloc((size_t) threads_number, sizeof(pthread_t));

    for (int i = 0; i < threads_number; ++i) {
        if (pthread_create(&thread_ids[i], NULL, work, NULL) != 0) {
            printf("Error while creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < threads_number; ++i) {
        if (pthread_join(thread_ids[i], NULL) != 0) {
            printf("Error while waiting for thread\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

void safe_open(char *filename) {
    if ((file = fopen(filename, "r")) == NULL) {
        printf("Cannot open file");
        exit(EXIT_FAILURE);
    }
}

void *work(void *arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    sleep(1);
    char *buffer = malloc((size_t) (1024 * records_number));
	
	pthread_testcancel();

    pthread_mutex_lock(&mutex);
    size_t red_rec = fread(buffer, 1024, (size_t) records_number, file);
    fflush(file);
    pthread_mutex_unlock(&mutex);

    while (red_rec > 0) {
        char *pos;
        if ((pos = strstr(buffer, pattern)) != NULL && !found) {
			found = true;
            unsigned long w = pos - buffer;
            w /= 1024;
            buffer += (w * 1024);
            //pthread_mutex_lock(&mutex);
            printf("word:\t\t%s\n", pattern);
            printf("num of line:\t%s\n", strtok(buffer, ":"));
            printf("thread id:\t%li\n", pthread_self());
            fflush(stdout);

            for (int j = 0; j < threads_number; ++j) {
                pthread_t mid = pthread_self();
                if (!pthread_equal(thread_ids[j], mid)) {
                    pthread_cancel(thread_ids[j]);
                }
            }
            //pthread_mutex_unlock(&mutex);
            return (void *) 0;
        }
		
	pthread_testcancel();

        pthread_mutex_lock(&mutex);
        red_rec = fread(buffer, 1024, (size_t) records_number, file);
        fflush(file);
        pthread_mutex_unlock(&mutex);
    }


    return (void *) 0;
}
