#include<iostream>
#include<sys/time.h>
#include"rand_matrix.h"
using namespace std;

void matrix_multply_with_block(float *a, float *b, float *c, int N, int B);


int main(int argc, char ** argv) {
    int N, B;
    float seed;
    cout << "enter seed(0-1):" << endl;
    cin >> seed;
    cout << "enter N & B:" << endl;
    while (cin >> N >> B) {
        // create matrix
        float *a = new float[N*N];
        float *b = new float[N*N];
        float *c = new float[N*N];
        matrix_gen(a, b, N, seed);
        // timer start
        struct timeval start;
        struct timeval end;
        unsigned long diff;
        gettimeofday(&start, NULL);
        // exec multply
        matrix_multply_with_block(a, b, c, N, B);
        // timer end
        gettimeofday(&end, NULL);
        diff = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        delete[] a;
        delete[] b;
        delete[] c;
        cout << "cost: " << diff << " ns" << endl;
        cout << "enter N & B:" << endl;
    }
    
    return 0;
}

void matrix_multply_with_block(float *a, float *b, float *c, int N, int B) {
    for (int i = 0; i < N; i += B) {
        for (int j = 0; j < N; j += B) {
            for (int k = 0; k < N; k += B) {
                // 计算小矩阵
                for (int ii = i; ii < i+B; ++ii) {
                    for (int jj = j; jj < j+B; ++jj) {
                        float sum = 0;
                        for (int kk = k; kk < k+B; ++kk) {
                            sum += a[ii*N + kk] * b[kk*N + jj];
                        }
                        c[ii*N + jj] = sum;
                    }
                }
            }
        }
    }
}
