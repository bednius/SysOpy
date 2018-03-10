#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "locks.h"

void print_error(char *msg);

void print_description();

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Type filename.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < strlen(argv[1]); ++i)
        if (argv[1][i] < 'a' || argv[1][i] > 'z')
            print_error("Wrong filename. Use only characters a-z.\n");

    int file = open(argv[1], O_RDWR);
    if (file <= 0) {
        print_error("erxdr");
    }
    print_description();
    int byte;
    int key;
    char character;

    while (1) {
        printf("What do you want to do?\n");
        scanf("%d", &key);

        switch (key) {
            case 1:
                printf("Type byte number: \n");
                scanf("%d", &byte);
                set_read_insta(byte, file);
                break;
            case 2:
                printf("Type byte number: \n");
                scanf("%d", &byte);
                set_read(byte, file);
                break;
            case 3:
                printf("Type byte number: \n");
                scanf("%d", &byte);
                set_write_insta(byte, file);
                break;
            case 4:
                printf("Type byte number: \n");
                scanf("%d", &byte);
                set_write(byte, file);
                break;
            case 5:
                printf("Type byte number: \n");
                scanf("%d", &byte);
                remove_lock(byte, file);
                break;
            case 6:
                printf("Type byte number: \n");
                scanf("%d", &byte);
                print_byte(byte,file);
                break;
            case 7:
                printf("Type byte number and character: \n");
                scanf("%d %c", &byte, &character);
                write_byte(byte, file, character);
                break;
            case 8:
                print_locks(file);
                break;
            case 0:
                close(file);
                exit(EXIT_SUCCESS);
            default:
                printf("Wrong argument.\n");
                print_description();
        }
    }
}

void print_description() {
    printf("1 - set lock on read\n");
    printf("2 - set lock on read(with waiting)\n");
    printf("3 - set lock on write\n");
    printf("4 - set lock on write(with waiting)\n");
    printf("5 - remove lock\n");
    printf("6 - print byte\n");
    printf("7 - write byte\n");
    printf("8 - print all locks\n");
    printf("0 - exit\n");
}

void print_error(char *msg) {
    printf(msg);
    exit(EXIT_FAILURE);
}