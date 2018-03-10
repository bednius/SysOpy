#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    srand((unsigned int) time(NULL));

    int records_number = 10000;
    int record_length = 1024;

    char *filename = "data";

    FILE *file = fopen(filename, "w");

    char buffer[record_length + 1];

    buffer[record_length - 1] = '\n';
    buffer[record_length] = '\0';

    for (int i = 0; i < records_number; ++i) {
        sprintf(buffer, "%04d:", i);
        for (int j = 5; j < record_length - 1; ++j) {
            buffer[j] = (char) (rand() % 26 + 'a');
        }
        fwrite(buffer, sizeof(char), (size_t) (record_length), file);
        fflush(file);
    }

    /*rewind(file);

    char *xd = malloc(sizeof(char) * 200);

    if(fread(xd, sizeof(char), (size_t) 42, file) != 5) {
        printf("XD\n");
    }

    printf("-\n%s\n-", xd);

    free(xd);*/

    fclose(file);
    return 0;
}
