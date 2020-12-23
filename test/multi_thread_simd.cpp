#include <nmmintrin.h>//SSE4.2(include smmintrin.h)
#include <immintrin.h>//AVX(include wmmintrin.h)
#include <opm.h>
#include "multi_thread_simd.h"
#define AVX_F_CAPACITY 8

// 使用网格划分的方法，不同的线程有着不同的 x, y，相当于只计算 C_xy 部分，矩阵长度为 len
// 程序将采用 split^2 的线程个数计算
void matrix_multply_avx_multi_grid(float *a, float *b, float *c, int N, int B) {
    // 暂时划分方案，等个sqrt(线程数量)
    split = 4;
    int len = N / split;
    // 暂时使用 16 个线程
    #pragma omp parallel(16)
    {
        // 获取当前线程号
        int cur_thread = omp_get_thread_num();
        // 获取当前线程需要计算的矩阵 C_xy
        int x = cur_thread / split;
        int y = cur_thread % split;
        // 计算 C_xy = A_xk * B_ky
        // 感觉这里其实也可以并行化，但是注意 C 是共享变量
        for (int k = 0; k + len <= N; k += len) {
            // 在长度为 len 的矩阵中进行 B 分块
            for (int i = x; i + B <= x + len; i += B) {
                for (int j = y; j + B <= y + len; j += B) {
                    for (int kk = k; kk + B <= k + len; kk += B) {

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
    }
}

