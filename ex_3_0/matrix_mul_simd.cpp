#include"matrix_mul_simd.h"

void matrix_multply_sse(float *a, float *b, float *c, int N, int B) {
    for (int i = 0; i < N; i += B) {
        for (int j = 0; j < N; j += B) {
            for (int k = 0; k < N; k += B) {
                // 计算小矩阵

            }
        }
    }
}