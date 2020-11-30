#include<iostream>
#include<sys/time.h>
#include"rand_matrix.h"
using namespace std;

void matrix_multply(float *a, float *b, float *c, int N);


int main(int argc, char ** argv) {
    int N;
    float seed;
    cout << "enter seed(0-1):" << endl;
    cin >> seed;
    cout << "enter N:" << endl;
    while (cin >> N) {
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
        matrix_multply(a, b, c, N);
        // timer end
        gettimeofday(&end, NULL);
        diff = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        delete[] a;
        delete[] b;
        delete[] c;
        cout << "cost: " << diff << " ns" << endl;
        cout << "enter N:" << endl;
    }
    
    return 0;
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