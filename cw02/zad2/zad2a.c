#include <stdio.h>
#include <stdlib.h>
#include "statf.h"

void check_args(int argc, const char **argv);

void print_error(char *msg);

int main(int argc, const char **argv) {
    check_args(argc, argv);
    tree_walk((char *) argv[1], atoi(argv[2]));
    return 0;
}

void check_args(int argc, const char **argv) {
    if (argc != 3)
        print_error("Wrong arguments.\n");
    if (atoi(argv[2]) < 0)
        print_error("Wrong arguments.\n");
}

void print_error(char *msg) {
    printf(msg);
    exit(EXIT_FAILURE);
}
