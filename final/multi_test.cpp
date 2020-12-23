#include<iostream>
#include<stdlib.h>
#include<sys/time.h>
#include<cstring>
#include<malloc.h>
#include"rand_matrix.h"
#include"multi_thread_simd.h"
using namespace std;

// avx 对齐要求
const int BYTE_SIZE = 32; 

// 分块大小 B
static int blocks[] = {8, 16, 32, 64, 128};
const int b_size = 5;
// 矩阵大小: N * N
static int matrixs[] = {512, 1024, 2048, 4096};
const int m_size = 4;

const float seed = 0.485739;

int main() {
    // 不同矩阵规模
    for (int i = 0; i < m_size; ++i) {
        int N = matrixs[i];
        // 不同分块大小
        for (int j = 0; j < b_size+1; ++j) {
            int B = j == b_size ? N : blocks[j];
            // avx 需要 256 位内存对齐
            float *a = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
            float *b = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
            float *c = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
            memset(c, 0, sizeof(float)*N*N);
            matrix_gen(a, b, N, seed);
//            cout << "随机矩阵构建结束，准备进行计算 ..."<< endl;

            // timer start
            struct timeval start;
            struct timeval end;
            unsigned long diff;
            gettimeofday(&start, NULL);
            // exec multply
            matrix_multply_avx_multi_grid(a, b, c, N, B);
            // timer end
            gettimeofday(&end, NULL);
            diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    
            // 打印结果
            cout << "多线程 AVX: 矩阵 N = " << N << " ， 分块 B = " << B << " ，耗时 : " << diff << "ms" << endl;
            // 释放内存
            free(a);
            free(b);
            free(c);
        }

    }
    return 0;
}


