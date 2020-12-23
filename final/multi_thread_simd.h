#pragma once

//  多线程(openmp) avx 版本，使用行划分的方法
void matrix_multply_avx_multi_grid(float *a, float *b, float *c, int N, int B);
