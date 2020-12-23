#include"rand_matrix.h"

float rand_float(float s) {
    return 4*s*(1-s);
}

void matrix_gen(float *a, float *b, int N, float seed) {
    float s = seed;
    for (int i = 0; i < N*N; ++i) {
        s = rand_float(s);
        a[i] = s;
        s = rand_float(s);
        b[i] = s;
    }
}