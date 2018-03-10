#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

const int BUFFER_SIZE = 256;
int R;
int **T;

void print_data() {
	char buffer[BUFFER_SIZE];
    FILE *data = fopen("data", "w+");
    if (data == NULL) {
        printf("fopen error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < R; ++i)
        for (int j = 0; j < R; ++j) {
            snprintf(buffer, BUFFER_SIZE, "%d %d %d\n", i, j, T[i][j]);
            if (fwrite(buffer, strlen(buffer), 1, data) != 1) {
                printf("fwrite error\n");
                exit(EXIT_FAILURE);
            }
        }
    fclose(data);
}

void plot() {
    char buffer[50];
    FILE *pipe;

    if ((pipe = popen("gnuplot", "w")) == NULL) {
        printf("popen error\n");
        exit(EXIT_FAILURE);
    }

    fprintf(pipe, "set view map\n");

    sprintf(buffer, "set xrange [0:%d]\n", R);
    fprintf(pipe, "%s", buffer);

    sprintf(buffer, "set yrange [0:%d]\n", R);
    fprintf(pipe, "%s", buffer);

    fprintf(pipe, "plot 'data' with image\n");

    fflush(pipe);
    getc(stdin);
    fclose(pipe);
}

int main(int argc, char **argv) {
    char *fifo = argv[1];
    R = atoi(argv[2]);
	char buffer[BUFFER_SIZE];

    if (mkfifo(fifo, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH) == -1) {
        printf("mkfifo error\n");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(fifo, "r");
    if (file == NULL) {
        printf("fopen error\n");
        exit(EXIT_FAILURE);
    }

	T = malloc(R * sizeof(int *));
    for (int i = 0; i < R; ++i)
        T[i] = malloc(R * sizeof(int));

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        int x = (int) (((atof(strtok(buffer, " ")) + 2.0) / 3.0) * R);
        int y = (int) (((atof(strtok(NULL, " ")) + 1.0) / 2.0) * R);
        T[x][y] = atoi(strtok(NULL, " "));
    }
    fclose(file);
    print_data();
    plot();
    remove(fifo);
    return 0;
}
