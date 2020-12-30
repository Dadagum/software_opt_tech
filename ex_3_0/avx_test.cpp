#include<iostream>
#include<stdlib.h>
#include<sys/time.h>
#include<cstring>
#include<malloc.h>
#include<omp.h>
#include<vector>
#include"rand_matrix.h"
#include"matrix_mul_avx.h"
using namespace std;

// avx 对齐要求
const int BYTE_SIZE = 32; 

// 默认参数
int N = 4096;
int B = 64;
float seed = 0.783943;

// 验证矩阵乘法
float verify(float* c) {
    vector<float> vt;
    for (int i = 0; i < N; ++i) {
        float cur_max = 0;
        for (int j = 0; j < N; ++j) {
            if (cur_max < c[i*N + j]) cur_max = c[i*N + j];
        }
        vt.push_back(cur_max);
        
    }

    float ans = vt[0];
    for (int i = 1; i < N; ++i) {
        if (ans > vt[i]) ans = vt[i];
    }
    return ans;
}

int main(int argc, char const *argv[])
{
    if (argc == 4) {
        N = atoi(argv[1]);
        B = atoi(argv[2]);
        seed = atof(argv[3]);
    }
    
    /* code */
    // avx 需要 256 位内存对齐
    float *a = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
    float *b = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
    float *c = (float*) memalign(BYTE_SIZE, sizeof(float)*N*N);
    memset(c, 0, sizeof(float)*N*N);
    matrix_gen(a, b, N, seed);

    // timer start
    struct timeval start;
    struct timeval end;
    unsigned long diff;
    gettimeofday(&start, NULL);
    // exec multply
    matrix_multply_avx(a, b, c, N, B);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);

    // 打印结果
    cout << "AVX: 矩阵 N = " << N << " ， 分块 B = " << B << " ，耗时 : " << diff << "us" << endl;
    cout << "verify value = " << verify(c) << endl;
    // 释放内存
    free(a);
    free(b);
    free(c);
    return 0;
}

