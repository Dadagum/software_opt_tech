#include<iostream>
#include<stdlib.h>
#include<sys/time.h>
#include<cstring>
#include<malloc.h>
#include"matrix_block.h"
#include"matrix_mul_simd.h"
#include"baseline.h"
#include"rand_matrix.h"
#include"multi_thread_simd.h"
using namespace std;

const int BYTE_SIZE = 16; 
// 矩阵大小: N * N
static int matrixs[] = {512};
const int m_size = 1;
int B = 16;

const float seed = 0.374424;

int main() {
    for (int m = 0; m < m_size; ++m) {
        int N = matrixs[m];
        cout << "正在比较 N = " << N << endl;
        // avx 需要 256 位内存对齐
        float *a = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
        float *b = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
        float *c1 = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
        float *c2 = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
        float *c3 = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
        memset(c1, 0, sizeof(float)*N*N);
        matrix_gen(a, b, N, seed);
        // baseline
        matrix_multply(a, b, c1, N);
        // for (int i = 0; i < N; ++i) {
        //     for (int j = 0; j < N; ++j) {
        //         cout << c[i*N + j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        // // 矩阵分块
        // memset(c, 0, sizeof(float)*N*N);
        //matrix_multiply_with_block(a,b,c,N,B);
        // for (int i = 0; i < N; ++i) {
        //     for (int j = 0; j < N; ++j) {
        //         cout << c[i*N + j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        // simd
         memset(c2, 0, sizeof(float)*N*N);
         matrix_multply_avx(a, b, c2, N, B);
        // for (int i = 0; i < N; ++i) {
        //     for (int j = 0; j < N; ++j) {
        //         cout << c[i*N + j] << " ";
        //     }
        //     cout << endl;
        // }

        // 多线程 simd
        memset(c3, 0, sizeof(float)*N*N);
        matrix_multply_avx_multi_grid(a, b, c3, N, B);
        // for (int i = 0; i < N; ++i) {
        //     for (int j = 0; j < N; ++j) {
        //         cout << c[i*N + j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        // 检查 baseline 计算是否一样
        for (int x = 0; x < N; ++x) {
            for (int y = 0; j < N; ++y) {
                if (c1[x*N + y] != c2[x*N + y] || c1[x*N + y] != c3[x*N + y]) {
                    cout << "出现错误：" << " x = " << x << " y = " << y << " 时，三个数分别为 " << c1[x*N + y] << " " << c2[x*N + y] << " " << c3[x*N + y] << endl;
                    break;
                }
            }
        }

        // 释放内存
        free(a);
        free(b);
        free(c1);
        free(c2);
        free(c3);
    }
    
}