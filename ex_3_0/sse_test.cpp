#include<iostream>
#include<malloc.h>
#include<sys/time.h>
#include"rand_matrix.h"
#include"matrix_mul_simd.h"
using namespace std;

// sse 对齐要求
const int BYTE_SIZE = 16; 

// 分块大小 B
static int blocks[] = {8, 16, 32, 64, 128};
const int b_size = 5;
// 矩阵大小: N * N
static int matrixs[] = {512, 1024, 2048, 4096};
const int m_size = 4;

const float seed = 0.374424;

int main(int argc, char ** argv) {
    for (int i = 0; i < m_size; ++i) {
        int N = matrixs[i];
        // sse 需要 128 位内存对齐
        float *a = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
        float *b = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
        float *c = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
        memset(c, 0, N*N);
        matrix_gen(a, b, N, seed);
        cout << "随机矩阵构建结束，准备进行计算 ..."<< endl;
        for (int j = 0; j < b_size; ++j) {
            int B = blocks[j];

            // timer start
            struct timeval start;
            struct timeval end;
            unsigned long diff;
            gettimeofday(&start, NULL);
            // exec multply
            matrix_multply_sse(a, b, c, N, B);
            // timer end
            gettimeofday(&end, NULL);
            diff =  (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    
            // 打印结果
            cout << "SSE: 矩阵 N = " << N << " ， 分块 B = " << B << " ，耗时 : " << diff << "s" << endl;
        }
        // 释放内存
        free(a);
        free(b);
        free(c);
    }
    return 0;
}


