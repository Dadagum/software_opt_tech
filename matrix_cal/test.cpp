#include<iostream>
#include<sys/time.h>
#include"rand_matrix.h"
#include"matrix_block.h"
#include"matrix_mul_avx.h"
#include<vector>
#include <algorithm>
#include <nmmintrin.h>//SSE4.2(include smmintrin.h)
#include <immintrin.h>//AVX(include wmmintrin.h)
using namespace std;

int blocks[] = {8, 16, 32, 64, 128};
int b_nums = 5;
int m_size[] = {512, 1024, 2048, 4096};
int m_nums = 4;

// baseline
void matrix_multply(float *a, float *b, float *c, int N);
// 单线程 simd
void single_matrix_multply_avx(float *a, float *b, float *c, int N, int B);


int main(int argc, char ** argv) {
    vector<unsigned long> base_time;
    // 规模 N 时的最佳运行秒
    vector<unsigned long> block_a_time;
    vector<unsigned long> avx_time;
    vector<unsigned long> mul_avx_time;
    int N, B;
    float seed = 0.925;
    struct timeval start;
    struct timeval end;
    unsigned long diff;
    // 1. 计算程序耗时
    for (int i = 0; i < m_nums; ++i) {
        N = m_size[i];
        cout << "N = " << N << endl;
        float *a = new float[N*N];
        float *b = new float[N*N];
        float *c = new float[N*N];
        matrix_gen(a, b, N, seed);
        // 基准程序
        cout << "baseline = ";
        gettimeofday(&start, NULL);
        matrix_multply(a, b, c, N);
        gettimeofday(&end, NULL);
        diff = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        base_time.push_back(diff);
        cout << diff << endl;

        vector<unsigned long> vt1, vt2, vt3;
        for (int j = 0; j < b_nums; ++j) {
            B = blocks[j];
            //cout << "B = " << B << endl;

            // 分块程序
            //cout << "fix a ... " << endl;
            gettimeofday(&start, NULL);
            matrix_multiply_with_block2(a,b,c,N,B);
            gettimeofday(&end, NULL);
            diff = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
            vt1.push_back(diff);

            // simd程序
            //cout << "avx ... " << endl;
            gettimeofday(&start, NULL);
            single_matrix_multply_avx(a,b,c,N,B);
            gettimeofday(&end, NULL);
            diff = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
            vt2.push_back(diff);

            // 多线程simd程序
            //cout << "mul avx ... " << endl;
            gettimeofday(&start, NULL);
            matrix_multply_avx(a,b,c,N,B);
            gettimeofday(&end, NULL);
            diff = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
            vt3.push_back(diff);
        }
        // 输出当前规模不同分块方案的耗时
        cout << "fix a :" << endl;
        for (int k = 0; k < vt1.size(); ++k) cout << vt1[k] << " ";
        cout << endl;

        cout << "avx :" << endl;
        for (int k = 0; k < vt2.size(); ++k) cout << vt2[k] << " ";
        cout << endl;

        cout << "mul avx :" << endl;
        for (int k = 0; k < vt3.size(); ++k) cout << vt3[k] << " ";
        cout << endl;

        // 记录最佳分块运行时间
        block_a_time.push_back(*min_element(vt1.begin(), vt1.end()));
        avx_time.push_back(*min_element(vt2.begin(), vt2.end()));
        mul_avx_time.push_back(*min_element(vt3.begin(), vt3.end()));

        delete[] a;
        delete[] b;
        delete[] c;
        cout << endl;
    }

    // 2. 计算加速比
    for (int i = 0; i < m_nums; ++i) {
        unsigned long fix = base_time[i];
        cout << "加速比计算 N = " << m_size[i] << " : " << block_a_time[i] / fix << " " << avx_time[i] / fix << " " << mul_avx_time[i] / fix << endl;;
    }
    // 3. 计算 gf
    for (int i = 0; i < m_nums; ++i) {
        int N = m_size[i];
        unsigned long total = 2 * N * N * N;
        cout << "浮点数总数 N = " << N << " 时 total = " << total << endl;
        cout << total * 1e6 / base_time[i] << " " << total * 1e6 / block_a_time[i] << " " << total * 1e6 / avx_time[i] << " " << total * 1e6 / mul_avx_time[i] << endl;
    }
}

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

void single_matrix_multply_avx(float *a, float *b, float *c, int N, int B) {
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
                        vc0 = _mm256_loadu_ps(&c[ii*N+jj]);
                        vc1 = _mm256_loadu_ps(&c[(ii+1)*N+jj]);
                        vc2 = _mm256_loadu_ps(&c[(ii+2)*N+jj]);
                        vc3 = _mm256_loadu_ps(&c[(ii+3)*N+jj]);
                        vc4 = _mm256_loadu_ps(&c[(ii+4)*N+jj]);
                        vc5 = _mm256_loadu_ps(&c[(ii+5)*N+jj]);
                        vc6 = _mm256_loadu_ps(&c[(ii+6)*N+jj]);
                        vc7 = _mm256_loadu_ps(&c[(ii+7)*N+jj]);

                        for (int kk = k; kk + AVX_F_CAPACITY <= k+B; kk += AVX_F_CAPACITY) {
                            vb = _mm256_loadu_ps(&b[kk*N + jj]);
                            // gcc 需要加上 -mfma 选项
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk]), vb, vc7);

                            vb = _mm256_loadu_ps(&b[(kk+1)*N + jj]);
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk + 1]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk + 1]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk + 1]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk + 1]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk + 1]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk + 1]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk + 1]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk + 1]), vb, vc7);

                            vb = _mm256_loadu_ps(&b[(kk+2)*N + jj]);
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk + 2]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk + 2]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk + 2]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk + 2]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk + 2]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk + 2]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk + 2]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk + 2]), vb, vc7);

 
                            vb = _mm256_loadu_ps(&b[(kk+3)*N + jj]);
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk + 3]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk + 3]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk + 3]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk + 3]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk + 3]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk + 3]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk + 3]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk + 3]), vb, vc7);

                            vb = _mm256_loadu_ps(&b[(kk+4)*N + jj]);
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk + 4]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk + 4]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk + 4]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk + 4]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk + 4]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk + 4]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk + 4]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk + 4]), vb, vc7);

                            vb = _mm256_loadu_ps(&b[(kk+5)*N + jj]);
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk + 5]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk + 5]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk + 5]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk + 5]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk + 5]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk + 5]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk + 5]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk + 5]), vb, vc7);

                            vb = _mm256_loadu_ps(&b[(kk+6)*N + jj]);
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk + 6]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk + 6]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk + 6]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk + 6]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk + 6]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk + 6]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk + 6]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk + 6]), vb, vc7);

                            vb = _mm256_loadu_ps(&b[(kk+7)*N + jj]);
                            vc0 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[ii*N + kk + 7]), vb, vc0);
                            vc1 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+1)*N + kk + 7]), vb, vc1);
                            vc2 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+2)*N + kk + 7]), vb, vc2);
                            vc3 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+3)*N + kk + 7]), vb, vc3);
                            vc4 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+4)*N + kk + 7]), vb, vc4);
                            vc5 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+5)*N + kk + 7]), vb, vc5);
                            vc6 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+6)*N + kk + 7]), vb, vc6);
                            vc7 = _mm256_fmadd_ps(_mm256_broadcast_ss(&a[(ii+7)*N + kk + 7]), vb, vc7);

                        }
                        // store
                        _mm256_storeu_ps(&c[ii*N+jj], vc0);
                        _mm256_storeu_ps(&c[(ii+1)*N+jj], vc1);
                        _mm256_storeu_ps(&c[(ii+2)*N+jj], vc2);
                        _mm256_storeu_ps(&c[(ii+3)*N+jj], vc3);
                        _mm256_storeu_ps(&c[(ii+4)*N+jj], vc4);
                        _mm256_storeu_ps(&c[(ii+5)*N+jj], vc5);
                        _mm256_storeu_ps(&c[(ii+6)*N+jj], vc6);
                        _mm256_storeu_ps(&c[(ii+7)*N+jj], vc7);
                    }
                }

            }
        }
    }
}