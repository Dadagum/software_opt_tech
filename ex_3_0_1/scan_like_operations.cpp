#include"scan_like_operations.h"
#include<immintrin.h>
// 使用 simd 处理的阈值，暂时定为 128
#define THRESHOLD 128
// avx 中可以容纳 8 个 32 位整数
#define AVX_S 8

// 调用者需要保证指针 x 后面有 8 个整数
inline __m256i SIMD_condition(int *x, int left, int right) {
    __m256i LEFT = _mm256_set1_epi32(left);
    __m256i RIGHT = _mm256_set1_epi32(right);
    __m256i X = _mm256_load_si256((__m256i*)x);
    return _mm256_and_si256(_mm256_cmpgt_epi32(RIGHT, X), _mm256_cmpgt_epi32(X, LEFT));
}

int find_first_match_simd(int *x, int *y, int size, int left, int right) {
    // 首先进行对齐
    
}

// 查看 x 数组中所有范围在 [left, right] 的 y 值列表
std::vector<int> find_all_matches_simd(int *x, int *y, int size, int left, int right);

int get_matches_sum_simd(int *x, int *y, int size, int left, int right);

int count_matches_simd(int *x, int size, int left, int right);

float get_matches_avg_simd(int *x, int *y, int size, int left, int right);

int get_matches_min(int *x, int *y, int size, int left, int right);

