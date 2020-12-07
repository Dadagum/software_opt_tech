#include <nmmintrin.h>//SSE4.2(include smmintrin.h)
#include <immintrin.h>//AVX(include wmmintrin.h)

#include"matrix_mul_simd.h"
#define SSE_F_CAPACITY 4
#define AVX_F_CAPACITY 8

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
                        // 手动计算 4*4 矩阵
                        // load
                        __m128 vc0, vc1, vc2, vc3, vb;
                        vc0 = _mm_load_ps(&c[ii*N+jj]);
                        vc1 = _mm_load_ps(&c[(ii+1)*N+jj]);
                        vc2 = _mm_load_ps(&c[(ii+2)*N+jj]);
                        vc3 = _mm_load_ps(&c[(ii+3)*N+jj]);
                        for (int kk = k; kk + SSE_F_CAPACITY <= k+B; kk += SSE_F_CAPACITY) {
                            vb = _mm_load_ps(&b[kk*N + jj]);
                            // gcc 需要加上 -mfma 选项
                            vc0 = _mm_fmadd_ps(_mm_broadcast_ss(&a[ii*N + kk]), vb, vc0);
                            vc1 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+1)*N + kk]), vb, vc1);
                            vc2 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+2)*N + kk]), vb, vc2);
                            vc3 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+3)*N + kk]), vb, vc3);

                            vb = _mm_load_ps(&b[(kk+1)*N + jj]);
                            vc0 = _mm_fmadd_ps(_mm_broadcast_ss(&a[ii*N + kk + 1]), vb, vc0);
                            vc1 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+1)*N + kk + 1]), vb, vc1);
                            vc2 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+2)*N + kk + 1]), vb, vc2);
                            vc3 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+3)*N + kk + 1]), vb, vc3);

                            vb = _mm_load_ps(&b[(kk+2)*N + jj]);
                            vc0 = _mm_fmadd_ps(_mm_broadcast_ss(&a[ii*N + kk + 2]), vb, vc0);
                            vc1 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+1)*N + kk + 2]), vb, vc1);
                            vc2 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+2)*N + kk + 2]), vb, vc2);
                            vc3 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+3)*N + kk + 2]), vb, vc3);

                            vb = _mm_load_ps(&b[(kk+3)*N + jj]);
                            vc0 = _mm_fmadd_ps(_mm_broadcast_ss(&a[ii*N + kk + 3]), vb, vc0);
                            vc1 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+1)*N + kk + 3]), vb, vc1);
                            vc2 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+2)*N + kk + 3]), vb, vc2);
                            vc3 = _mm_fmadd_ps(_mm_broadcast_ss(&a[(ii+3)*N + kk + 3]), vb, vc3);

                        }
                        // store
                        _mm_store_ps(&c[ii*N+jj], vc0);
                        _mm_store_ps(&c[(ii+1)*N+jj], vc1);
                        _mm_store_ps(&c[(ii+2)*N+jj], vc2);
                        _mm_store_ps(&c[(ii+3)*N+jj], vc3);
                    }
                }

            }
        }
    }
}

// avx : 256 位寄存器可以存放 8 个 float
// 需要 B % 8 == 0
void matrix_multply_avx(float *a, float *b, float *c, int N, int B) {
// 固定矩阵 A
    for (int i = 0; i < N; i += B) {
        for (int j = 0; j < N; j += B) {
            for (int k = 0; k < N; k += B) {
                // 计算小矩阵
                for (int ii = i; ii + AVX_F_CAPACITY <= i+B; ii += AVX_F_CAPACITY) {
                    for (int jj = j; jj + AVX_F_CAPACITY <= j+B; jj += AVX_F_CAPACITY) {
                        // 手动计算 8*8 矩阵
                        // load
                        __m256 vc0, vc1, vc2, vc3, vc4, vc5, vc6, vc7, vc8, vb;
                        vc0 = _mm256_load_ps(&c[ii*N+jj]);
                        vc1 = _mm256_load_ps(&c[(ii+1)*N+jj]);
                        vc2 = _mm256_load_ps(&c[(ii+2)*N+jj]);
                        vc3 = _mm256_load_ps(&c[(ii+3)*N+jj]);
                        vc4 = _mm256_load_ps(&c[(ii+4)*N+jj]);
                        vc5 = _mm256_load_ps(&c[(ii+5)*N+jj]);
                        vc6 = _mm256_load_ps(&c[(ii+6)*N+jj]);
                        vc7 = _mm256_load_ps(&c[(ii+7)*N+jj]);

                        for (int kk = k; kk + AVX_F_CAPACITY <= k+B; kk += AVX_F_CAPACITY) {
                            vb = _mm256_load_ps(&b[kk*N + jj]);
                            // gcc 需要加上 -mfma 选项
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk]), vb, vc7);

                            vb = _mm256_load_ps(&b[(kk+1)*N + jj]);
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk + 1]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk + 1]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk + 1]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk + 1]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk + 1]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk + 1]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk + 1]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk + 1]), vb, vc7);

                            vb = _mm256_load_ps(&b[(kk+2)*N + jj]);
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk + 2]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk + 2]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk + 2]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk + 2]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk + 2]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk + 2]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk + 2]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk + 2]), vb, vc7);

 
                            vb = _mm256_load_ps(&b[(kk+3)*N + jj]);
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk + 3]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk + 3]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk + 3]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk + 3]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk + 3]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk + 3]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk + 3]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk + 3]), vb, vc7);

                        }
                        // store
                        _mm256_store_ps(&c[ii*N+jj], vc0);
                        _mm256_store_ps(&c[(ii+1)*N+jj], vc1);
                        _mm256_store_ps(&c[(ii+2)*N+jj], vc2);
                        _mm256_store_ps(&c[(ii+3)*N+jj], vc3);
                        _mm256_store_ps(&c[(ii+4)*N+jj], vc4);
                        _mm256_store_ps(&c[(ii+5)*N+jj], vc5);
                        _mm256_store_ps(&c[(ii+6)*N+jj], vc6);
                        _mm256_store_ps(&c[(ii+7)*N+jj], vc7);
                    }
                }

            }
        }
    }
}