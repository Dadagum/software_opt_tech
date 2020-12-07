#include <nmmintrin.h>//SSE4.2(include smmintrin.h)
#include <immintrin.h>//AVX(include wmmintrin.h)

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