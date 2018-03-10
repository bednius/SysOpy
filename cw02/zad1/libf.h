//
// Created by kreska on 19.03.17.
//

#ifndef SO02_LIBF_H
#define SO02_LIBF_H

void generate(const char *filename, long long int record_size, long long int num_of_records);

void lib_sort(const char *filename, long long int record_size, long long int num_of_lines);

void lib_shuffle(const char *filename, long long int record_size, long long int num_of_records);

#endif //SO02_LIBF_H
