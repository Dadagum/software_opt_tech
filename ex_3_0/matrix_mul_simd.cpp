#include<emmintrin.h>
#include"matrix_mul_simd.h"
#define SSE_F_CAPACITY 4

// sse：128 位寄存器可以放 4 个 float
// 需要 B % 4 == 0
void matrix_multply_sse(float *a, float *b, float *c, int N, int B) {
    // 固定矩阵 A
    for (int i = 0; i < N; i += B) {
        for (int j = 0; j < N; j += B) {
            for (int k = 0; k < N; k += B) {
                // 计算小矩阵
                for (int ii = i; ii + SSE_F_CAPACITY <= i+B; ii += SSE_F_CAPACITY) {
                    for (int jj = j; jj + SSE_F_CAPACITY <= j+B; jj += SSE_F_CAPACITY) {
                        for (int kk = k; kk + SSE_F_CAPACITY <= k+B; kk += SSE_F_CAPACITY) {
                            
                        }
                    }
                }

            }
        }
    }
}