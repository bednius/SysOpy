//
// Created by kreska on 26.03.17.
//

#ifndef UNTITLED8_LOCKS_H
#define UNTITLED8_LOCKS_H

void set_read_insta(int byte, int file);

void set_read(int byte, int file);

void set_write_insta(int byte, int file);

void set_write(int byte, int file);

void print_locks(int file);

void remove_lock(int byte, int file);

void print_byte(int byte, int file);

void write_byte(int byte, int file, char character);

#endif //UNTITLED8_LOCKS_H
