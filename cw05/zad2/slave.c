#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <complex.h>

double complex rand_complex() {
    double re = ((double)((rand() % (RAND_MAX - 1)) + 1) / RAND_MAX) * 3 - 2;
    double im = ((double)((rand() % (RAND_MAX - 1)) + 1) / RAND_MAX) * 2 - 1;
    double complex c = re + im * I;
    return c;
}

int iters(double complex p, int K) {
    double complex z = 0;
    int i;
    for (i = 0; i < K && cabs(z) <= 2; ++i)
        z = z * z + p;
    return i;
}

int main(int argc, char **argv) {
    srand((unsigned int) time(NULL));
    const int BUFFER_SIZE = 256;
    char *pipe_path = argv[1];
    int N = atoi(argv[2]);
    int K = atoi(argv[3]);

    FILE *file;
    if ((file = fopen(pipe_path, "w")) == NULL) {
        printf("Cannot open pipe\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; ++i) {
    	double complex z = rand_complex();
        char buffer[BUFFER_SIZE];
        snprintf(buffer, BUFFER_SIZE, "%.10f %.10f %d\n", creal(z), cimag(z), iters(z, K));
        fwrite(&buffer, strlen(buffer), 1, file);
        fflush(file);
    }
    fclose(file);
    return 0;
}
