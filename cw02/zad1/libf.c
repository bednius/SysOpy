//
// Created by kreska on 19.03.17.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void generate(const char *filename, long long int record_size, long long int num_of_records) {
    FILE *file;

    if ((file = fopen(filename, "w")) == NULL) {
        printf("Cannot open/create file for overwrite.");
        fclose(file);
        exit(EXIT_FAILURE);
    } // Open file for overwrite. The file is created if it does not exist.

    FILE *urandom;

    if (!(urandom = fopen("/dev/urandom", "r"))) {
        printf("Cannot open urandom file");
        fclose(urandom);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    unsigned char *record = malloc(sizeof(char) * (record_size + 1));
    record[record_size] = '\n';

    for (int i = 0; i < num_of_records; ++i) {
        if (fread(record, sizeof(char), (size_t) record_size, urandom) != record_size) {
            printf("Error while reading urandom file.\n");
            free(record);
            fclose(file);
            fclose(urandom);
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < record_size; ++j) {
            record[j] = (unsigned char) (record[j] % 26 + 'a');
        }
        if (fwrite(record, sizeof(char), (size_t) (record_size + 1), file) != record_size + 1) {
            printf("Error while writing to %s file.\n", filename);
            free(record);
            fclose(file);
            fclose(urandom);
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
    fclose(urandom);
    free(record);
}

void lib_sort(const char *filename, long long int record_size, long long int num_of_lines) {
    record_size++;  // '\n' char at the end of line
    FILE *file;

    if (!(file = fopen(filename, "r+b"))) {
        printf("Cannot open %s file.\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char *buffer1 = malloc((size_t) record_size);
    char *buffer2 = malloc((size_t) record_size);

    bool flag = true;

    while (flag) {
        flag = false;
        rewind(file);

        fread(buffer1, (size_t) record_size, 1, file);
        fseek(file, 0,  SEEK_CUR);

        for (int i = 1; i < num_of_lines; ++i) {
            fread(buffer2, (size_t) record_size, 1, file);
            fseek(file, 0,  SEEK_CUR);

            if (buffer1[0] > buffer2[0]) {
                fseek(file, record_size * (-2), SEEK_CUR);
                fwrite(buffer2, (size_t) record_size, 1, file);
                fseek(file, 0,  SEEK_CUR);
                fwrite(buffer1, (size_t) record_size, 1, file);
                fseek(file, 0,  SEEK_CUR);
                flag = true;
            } else {
                char *tmp = buffer2;
                buffer2 = buffer1;
                buffer1 = tmp;
            }
        }
        num_of_lines--;
    }
    fclose(file);
    free(buffer1);
    free(buffer2);
}

void lib_shuffle(const char *filename, long long int record_size, long long int num_of_records) {
    FILE *file;
    record_size++;

    if (!(file = fopen(filename, "r+"))) {
        printf("Cannot open %s file\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char *buffer1 = malloc((size_t) record_size);
    char *buffer2 = malloc((size_t) record_size);


    for (int i = 0; i < num_of_records - 1; ++i) {
        int j = (int) (rand() % (num_of_records - i) + i);
        fread(buffer1, (size_t) record_size, 1, file);
        fseek(file, j * record_size, SEEK_SET);
        fread(buffer2, (size_t) record_size, 1, file);
        fseek(file, j * record_size, SEEK_SET);
        fwrite(buffer1, (size_t) record_size, 1, file);
        fseek(file, i * record_size, SEEK_SET);
        fwrite(buffer2, (size_t) record_size, 1, file);
        fseek(file, 0, SEEK_CUR);
    }
    fclose(file);
    free(buffer1);
    free(buffer2);
}




