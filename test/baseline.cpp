#include"baseline.h"

void matrix_multply(float *a, float *b, float *c, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float sum = 0.0;
            for (int k = 0; k < N; ++k) {
                sum += a[i*N + k] * b[k*N + j];
            }
            c[i*N + j] = sum;
        }
    }
}