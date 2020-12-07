#pragma once

void matrix_multply_sse(float *a, float *b, float *c, int N, int B);
void matrix_multply_avx(float *a, float *b, float *c, int N, int B);