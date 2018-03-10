#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libf.h"
#include "sysf.h"
#include <sys/resource.h>
#include <time.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void check_times(clock_t *real, struct rusage *usage);

void check_args(int argc, const char **argv);

void print_error(char *msg);

void run(const char *type, const char *string, const char *filename, int lines, int size);

void copy_file(char *file1, char *file2);

void make_tests(const int record_size, const int num_of_rec);


int main(int argc, char const *argv[]) {
    srand((unsigned int) time(NULL));
    check_args(argc, argv);
    run(argv[1], argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
    return 0;
}

void run(const char *type, const char *fun, const char *filename, int num_of_lines, int record_size) {
    if (strcmp(fun, "generate") == 0) {
        generate(filename, record_size, num_of_lines);
        return;
    }
    if (strcmp(type, "sys") == 0) {
        if (strcmp(fun, "shuffle") == 0)
            sys_shuffle(filename, record_size, num_of_lines);
        else
            sys_sort(filename, record_size, num_of_lines);
    } else {
        if (strcmp(fun, "shuffle") == 0)
            lib_shuffle(filename, record_size, num_of_lines);
        else
            lib_sort(filename, record_size, num_of_lines);
    }
}

void check_args(int argc, const char **argv) {
    if (argc == 2) {
        if (strcmp(argv[1], "test") == 0) {
            make_tests(4, 8000);
            make_tests(512, 6000);
            make_tests(4096, 4000);
            make_tests(8192, 3000);
        }
	exit(0);
    }

    if (argc != 6)
        print_error("Wrong number of arguments. Type 5.\n");

    int iterator = 1;

    if (!((strcmp(argv[iterator], "sys") == 0) || (strcmp(argv[iterator], "lib") == 0)))
        print_error("Wrong first argument. Type sys or lib.\n");

    ++iterator;

    if (!((strcmp(argv[iterator], "generate") == 0) || (strcmp(argv[iterator], "sort") == 0) ||
          (strcmp(argv[iterator], "shuffle") == 0)))
        print_error("Wrong second argument. Type one of: generate, sort, shuffle.\n");

    ++iterator;

    for (int i = 0; i < strlen(argv[iterator]); ++i)
        if (argv[iterator][i] < 'a' || argv[iterator][i] > 'z')
            print_error("Wrong filename. Use only characters a-z.\n");

    ++iterator;


    for (int i = 0; i < strlen(argv[iterator]); ++i)
        if (argv[iterator][i] < '0' || argv[iterator][i] > '9')
            print_error("Wrong number of lines.\n");

    ++iterator;

    for (int i = 0; i < strlen(argv[iterator]); ++i)
        if (argv[iterator][i] < '0' || argv[iterator][i] > '9')
            print_error("Wrong size of record.\n");
}

void print_error(char *msg) {
    printf("%s", msg);
    exit(EXIT_FAILURE);
}

void check_times(clock_t *real, struct rusage *usage) {
    clock_t curr_real;
    struct rusage curr_usage;

    curr_real = clock();
    getrusage(RUSAGE_SELF, &curr_usage);

    printf("Current time:\t\tR:\t%.6f\t\tU:\t%.6f\t\tS:\t%.6f\n",
           (double) (curr_real) / CLOCKS_PER_SEC,
           curr_usage.ru_utime.tv_sec + curr_usage.ru_utime.tv_usec / 1e6,
           curr_usage.ru_stime.tv_sec + curr_usage.ru_stime.tv_usec / 1e6);

    printf("From prev check:\tR:\t%.6f\t\tU:\t%.6f\t\tS:\t%.6f\n\n",
           (double) (curr_real - (*real)) / CLOCKS_PER_SEC,
           (curr_usage.ru_utime.tv_sec - (*usage).ru_utime.tv_sec) +
           (curr_usage.ru_utime.tv_usec - (*usage).ru_utime.tv_usec) / 1e6,
           (curr_usage.ru_stime.tv_sec - (*usage).ru_stime.tv_sec) +
           (curr_usage.ru_stime.tv_usec - (*usage).ru_stime.tv_usec) / 1e6);

    (*real) = curr_real;
    (*usage) = curr_usage;
}

void copy_file(char *file1, char *file2) {
    int source = open(file1, O_RDONLY, 0);
    int dest = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    struct stat stat_source;
    fstat(source, &stat_source);
    sendfile(dest, source, 0, (size_t) stat_source.st_size);
    close(source);
    close(dest);
}

void make_tests(const int record_size, const int num_of_rec) {
    char *file1 = "file1";
    char *file2 = "file2";

    clock_t real = clock();
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    printf("Checkpoint\n");
    check_times(&real, &usage);

    generate(file1, record_size, num_of_rec);
    printf("File has been created(record_size = %d, num_of_rec = %d)\n", record_size, num_of_rec);
    check_times(&real, &usage);
    copy_file(file1, file2);
    printf("File has been copied\n");
    check_times(&real, &usage);
    lib_sort(file1, record_size, num_of_rec);
    printf("Lib sorting\n");
    check_times(&real, &usage);
    sys_sort(file2, record_size, num_of_rec);
    printf("Sys sorting\n");
    check_times(&real, &usage);
    printf("\n\n\n\n\n");
}
