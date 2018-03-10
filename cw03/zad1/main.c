
#include <stdio.h>
#include <string.h>
#include "interpreter.h"

int main(int argc, char* argv[]) {
    if(argc < 2)
        print_error("Wrong arguments.\n");
    for (int i = 0; i < strlen(argv[1]); ++i)
        if (argv[1][i] < 'a' || argv[1][i] > 'z')
            print_error("Wrong filename. Use only characters a-z.\n");
    run_interpreter(*(argv+1));
    return 0;;
}