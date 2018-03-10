#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include "nftwf.h"

void check_args(int argc, const char **argv);

void print_error(char *msg);

int main(int argc, const char **argv) {
    check_args(argc, argv);
    init_max(atoi(argv[2]));
    if (nftw(argv[1], display_info, 20, FTW_DEPTH) == -1) {
        print_error("nftw problem.\n");
    }
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
