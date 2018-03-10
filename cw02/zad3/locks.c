//
// Created by kreska on 26.03.17.
//

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


struct flock *create_flock(int byte, short type) {
    struct flock *lock = malloc(sizeof(struct flock));
    lock->l_len = 1;
    lock->l_type = type;
    lock->l_whence = SEEK_SET;
    lock->l_start = byte;
    return lock;
}

void set_read_insta(int byte, int file) {
    struct flock *lock = create_flock(byte, F_RDLCK);
    if (fcntl(file, F_SETLK, lock) == -1) {
        printf("Cannot set lock.\n");
        free(lock);
    }
}

void set_read(int byte, int file) {
    struct flock *lock = create_flock(byte, F_RDLCK);
    if (fcntl(file, F_SETLKW, lock) == -1) {
        printf("Cannot set lock.\n");
        free(lock);
    }

}

void set_write_insta(int byte, int file) {
    struct flock *lock = create_flock(byte, F_WRLCK);
    if (fcntl(file, F_SETLK, lock) == -1) {
        printf("Cannot set lock.\n");
        free(lock);
    }
}

void set_write(int byte, int file) {
    struct flock *lock = create_flock(byte, F_WRLCK);
    if (fcntl(file, F_SETLKW, lock) == -1) {
        printf("Cannot set lock.\n");
        free(lock);
    }
}

void remove_lock(int byte, int file) {
    struct flock *lock = create_flock(byte, F_UNLCK);
    if (fcntl(file, F_SETLK, lock) == -1) {
        printf("Cannot remove lock.\n");
        free(lock);
    }
}

void print_byte(int byte, int file) {
    lseek(file, byte, SEEK_SET);
    char character;
    if (read(file, &character, 1) < 1)
        printf("Cannot read character.\n");
    else
        printf("Read character: %c\n", character);
}

void write_byte(int byte, int file, char character) {
    lseek(file, byte, SEEK_SET);
    if (write(file, &character, 1) != 1)
        printf("Cannot write character.\n");
    else
        printf("Written character: %c\n", character);
}

void print_locks(int file) {
    printf("Locks: \n");
    struct flock *lock = malloc(sizeof(struct flock));
    int size = (int) lseek(file, 0, SEEK_END);
    for (int i = 0; i < size; i++) {
        lock->l_len = 1;
        lock->l_type = F_WRLCK;
        lock->l_whence = SEEK_SET;
        lock->l_start = i;
        fcntl(file, F_GETLK, lock);
        if (lock->l_type == F_RDLCK)
            printf("Byte:\t%d\t\tPID:\t%d\t\tType:\tWRITE\n", i, lock->l_pid);
        else if (lock->l_type == F_WRLCK)
            printf("Byte:\t%d\t\tPID:\t%d\t\tType:\tREAD\n", i, lock->l_pid);
    }
    free(lock);
}
