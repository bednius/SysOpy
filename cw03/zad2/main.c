
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "interpreter.h"

#define MAX_MEMORY_LIMIT 200
#define MAX_CPU_LIMIT 300

int main(int argc, char *argv[]) {
    if (argc != 4)
        print_error("Wrong number of arguments.\n");
    for (int i = 0; i < strlen(argv[1]); ++i)
        if (argv[1][i] < 'a' || argv[1][i] > 'z')
            print_error("Wrong filename. Use only characters a-z.\n");
    int memory_limit = atoi(argv[2]);
    int cpu_limit = atoi(argv[3]);
    if (memory_limit < 0 || memory_limit > MAX_MEMORY_LIMIT)
        print_error("Wrong memory limit.\n");
    if (cpu_limit < 0 || cpu_limit > MAX_CPU_LIMIT)
        print_error("Wrong CPU limit.\n");
    run_interpreter(argv[1], memory_limit, cpu_limit);
    return 0;
}