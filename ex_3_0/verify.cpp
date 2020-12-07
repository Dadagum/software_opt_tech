#include<iostream>
#include"matrix_mul_simd.h"
using namespace std;

static float a[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; 
static float b[] = {10,11, 12,13,14,15,16,17,18,19,20,21,22,23,24,25}; 
static float c[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const int N = 4;
const int B = 4;

int main() {
    matrix_multply_sse(a,b,c,N,B);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << c[i*N + j] << " ";
        }
        cout << endl;
    }
}