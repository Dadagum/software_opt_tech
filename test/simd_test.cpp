#include<iostream>
#include<stdlib.h>
#include<sys/time.h>
#include<cstring>
#include<malloc.h>
#include"matrix_block.h"
#include"matrix_mul_simd.h"
#include"baseline.h"
#include"rand_matrix.h"
using namespace std;

const int BYTE_SIZE = 8; 
// 矩阵大小: N * N
static int matrixs[] = {8, 16, 24};
const int m_size = 3;
int B = 8;

const float seed = 0.374424;

int main() {
    for (int m = 0; m < m_size; ++m) {
        int N = matrixs[m];
        cout << "正在比较 N = " << N << endl;
        // sse 需要 128 位内存对齐
        float *a = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
        float *b = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
        float *c = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
        memset(c, 0, sizeof(float)*N*N);
        matrix_gen(a, b, N, seed);
        // baseline
        matrix_multply(a, b, c, N);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << c[i*N + j] << " ";
            }
            cout << endl;
        }
        cout << endl;

        // 矩阵分块
        memset(c, 0, sizeof(float)*N*N);
        matrix_multiply_with_block(a,b,c,N,B);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << c[i*N + j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        memset(c, 0, sizeof(float)*N*N);
        // simd
        matrix_multply_sse(a, b, c, N, B);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << c[i*N + j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        // 释放内存
        free(a);
        free(b);
        free(c);
    }
    
}