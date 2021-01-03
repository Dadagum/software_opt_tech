#include"scan_like_operations.h"
#include <nmmintrin.h>
#include <immintrin.h>
#include<iostream>

int find_first_match_simd(int *x, int *y, int size, int left, int right) {
    // 首先进行对齐(暂时不做)
    __m256i LEFT = _mm256_set1_epi32(left-1);
    __m256i RIGHT = _mm256_set1_epi32(right+1);
    int i = 0;
    for (; i + AVX_S <= size; i += AVX_S) {
        __m256i X = _mm256_loadu_si256((__m256i*)(x+i));
        // SIMD_condition
        __m256i con = _mm256_and_si256(_mm256_cmpgt_epi32(RIGHT, X), _mm256_cmpgt_epi32(X, LEFT));
        
        // 除了 AVX-512 貌似没有办法转换为掩码，因此只好暂时出此下册，在内存中手动算
        int mask = _mm256_movemask_epi8(con);
        if (mask != 0) {
            for (int j = 0; j < 8; ++j) {
                if (mask & 1) return *(y+i+j);
                mask >>= 4;
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

    __m256i LEFT = _mm256_set1_epi32(left-1);
    __m256i RIGHT = _mm256_set1_epi32(right+1);
    __m256i X, con;
    int i = 0;
    for (; i + AVX_S <= size; i += AVX_S) {
        X = _mm256_loadu_si256((__m256i*)(x+i));
        // SIMD_condition
        con = _mm256_and_si256(_mm256_cmpgt_epi32(RIGHT, X), _mm256_cmpgt_epi32(X, LEFT));
        int mask = _mm256_movemask_epi8(con);
        if (mask != 0) {
            for (int j = 0; j < 8; ++j) {
                if (mask & 1) res.push_back(*(y+i+j));
                mask >>= 4;
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

    __m256i LEFT = _mm256_set1_epi32(left-1);
    __m256i RIGHT = _mm256_set1_epi32(right+1);
    __m256i X, con;
    int i = 0;
    for (; i + AVX_S <= size; i += AVX_S) {
        X = _mm256_loadu_si256((__m256i*)(x+i));
        // SIMD_condition
        con = _mm256_and_si256(_mm256_cmpgt_epi32(RIGHT, X), _mm256_cmpgt_epi32(X, LEFT));
    
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


