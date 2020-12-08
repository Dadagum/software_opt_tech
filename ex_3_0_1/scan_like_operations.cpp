#include"scan_like_operations.h"
#include <nmmintrin.h>
#include <immintrin.h>
#include<iostream>
using namespace std;

static int mask_buffer[AVX_S];

int find_first_match_simd(int *x, int *y, int size, int left, int right) {
    // 首先进行对齐(暂时不做)

    
    __m256i LEFT = _mm256_set1_epi32(left);
    __m256i RIGHT = _mm256_set1_epi32(right);
    int i = 0;
    for (; i + AVX_S <= size; i += AVX_S) {
        __m256i X = _mm256_loadu_si256((__m256i*)(x+i));
        // SIMD_condition
        __m256i con = _mm256_and_si256(_mm256_cmpgt_epi32(RIGHT, X), _mm256_cmpgt_epi32(X, LEFT));
        
        // 除了 AVX-512 貌似没有办法转换为掩码，因此只好暂时出此下册，在内存中手动算
        _mm256_storeu_si256((__m256i*)mask_buffer, con);
        int mask = 0;
        for (int i = 0; i < AVX_S; ++i) {
                int flag = mask_buffer[i] != 0;
                mask += flag << i;
        }
        if (mask != 0) {
            for (int j = 0; j < AVX_S; ++j) {
                if (mask >> j & 1){
                    return *(y+i+j);
                }
            }
            
        }
    }

    // 说明前面都没有 match，那么对剩下的部分计算
    for (; i < size; ++i) {
        if (left <= x[i] && x[i] <= right){
            return y[i];
        }
    }
    return NOT_EXSIST;
}

// 查看 x 数组中所有范围在 [left, right] 的 y 值列表
std::vector<int> find_all_matches_simd(int *x, int *y, int size, int left, int right) {
    std::vector<int> res;

    __m256i LEFT = _mm256_set1_epi32(left);
    __m256i RIGHT = _mm256_set1_epi32(right);
    int i = 0;
    for (; i + AVX_S <= size; i += AVX_S) {
        __m256i X = _mm256_loadu_si256((__m256i*)(x+i));
        // SIMD_condition
        __m256i con = _mm256_and_si256(_mm256_cmpgt_epi32(RIGHT, X), _mm256_cmpgt_epi32(X, LEFT));
        
        // 除了 AVX-512 貌似没有办法转换为掩码，因此只好暂时出此下册，在内存中手动算
        _mm256_storeu_si256((__m256i*)mask_buffer, con);
        int mask = 0;
        for (int i = 0; i < AVX_S; ++i) {
                int flag = mask_buffer[i] != 0;
                mask += flag << i;
        }
        if (mask != 0) {
            for (int j = 0; j < AVX_S; ++j) {
                if (mask >> j & 1){
                    res.push_back(*(y+i+j));
                }
            }
            
        }
    }

    // 说明前面都没有 match，那么对剩下的部分计算
    for (; i < size; ++i) {
        if (left <= x[i] && x[i] <= right){
            res.push_back(y[i]);
        }
    }
    return res;
}

int get_matches_sum_simd(int *x, int *y, int size, int left, int right) {
    int sum = 0;

    __m256i LEFT = _mm256_set1_epi32(left);
    __m256i RIGHT = _mm256_set1_epi32(right);
    int i = 0;
    for (; i + AVX_S <= size; i += AVX_S) {
        __m256i X = _mm256_loadu_si256((__m256i*)(x+i));
        // SIMD_condition
        __m256i con = _mm256_and_si256(_mm256_cmpgt_epi32(RIGHT, X), _mm256_cmpgt_epi32(X, LEFT));
        

    }

    // 说明前面都没有 match，那么对剩下的部分计算
    for (; i < size; ++i) {
        if (left <= x[i] && x[i] <= right){
            return y[i];
        }
    }
    return sum;
}

int count_matches_simd(int *x, int size, int left, int right);

float get_matches_avg_simd(int *x, int *y, int size, int left, int right);

int get_matches_min(int *x, int *y, int size, int left, int right);


