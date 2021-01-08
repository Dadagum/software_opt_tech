#include"matrix_block.h"

void matrix_multiply_with_block(float *a, float *b, float *c, int N, int B) {
    // 固定矩阵 C
    for (int i = 0; i < N; i += B) {
        for (int j = 0; j < N; j += B) {
            for (int k = 0; k < N; k += B) {
                // 计算小矩阵
                for (int ii = i; ii < i+B; ++ii) {
                    for (int jj = j; jj < j+B; ++jj) {
                        float sum = 0;
                        for (int kk = k; kk < k+B; ++kk) {
                            sum += a[ii*N + kk] * b[kk*N + jj];
                        }
                        c[ii*N + jj] += sum;
                    }
                }
            }
        }
    }
}

void matrix_multiply_with_block2(float *a, float *b, float *c, int N, int B) {
    // 固定矩阵 A
    for (int i = 0; i < N; i += B) {
        for (int j = 0; j < N; j += B) {
            for (int k = 0; k < N; k += B) {
                // 计算小矩阵
                for (int ii = i; ii < i+B; ++ii) {
                    for (int kk = k; kk < k+B; ++kk) {
                        int fix = a[ii*N + kk];
                        for (int jj = j; jj < j+B; ++jj) {
                            c[ii*N + jj] += fix * b[kk*N + jj];
                        }
                    }
                }
            }
        }
    }
}

void matrix_multiply_with_block3(float *a, float *b, float *c, int N, int B) {
    // 固定矩阵 B
    for (int i = 0; i < N; i += B) {
        for (int j = 0; j < N; j += B) {
            for (int k = 0; k < N; k += B) {
                // 计算小矩阵
                for (int jj = j; jj < j+B; ++jj) {
                    for (int kk = k; kk < k+B; ++kk) {
                        int fix = b[kk*N + jj];
                        for (int ii = i; ii < i+B; ++ii) {
                            c[ii*N + jj] += fix * a[ii*N + kk];
                        }
                    }
                }
            }
        }
    }
}


