#pragma once
#define AVX_F_CAPACITY 8

void matrix_multply_avx(float *a, float *b, float *c, int N, int B);