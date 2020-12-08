#include"scan_like_operations.h"
#include <nmmintrin.h>
#include <immintrin.h>
// 使用 simd 处理的阈值，暂时定为 128
#define THRESHOLD 128
// avx 中可以容纳 8 个 32 位整数
#define AVX_S 8
#define AVX_ALIGN 32
#define NOT_EXSIST -2147483648

static int mask_buffer[AVX_S+10];

// 调用者需要保证指针 x 后面有 8 个整数
inline __m256i SIMD_condition(int *x, int left, int right) {
    __m256i LEFT = _mm256_set1_epi32(left);
    __m256i RIGHT = _mm256_set1_epi32(right);
    __m256i X = _mm256_loadu_si256((__m256i*)x);
    return _mm256_and_si256(_mm256_cmpgt_epi32(RIGHT, X), _mm256_cmpgt_epi32(X, LEFT));
}

// 除了 avx-512 支持，貌似没有函数能够直接转化为 mask，只能暂时手动在内存中转化，找到再改
static int SIMD_bit_vector(__m256i x) {
    _mm256_storeu_si256((__m256i*)mask_buffer, x);
    int mask = 0;
    for (int i = 0; i < AVX_ALIGN; ++i) {
        int flag = mask_buffer[i] != 0;
        mask += flag << i;
	}
    return mask;
}

int find_first_match_simd(int *x, int *y, int size, int left, int right) {
    // 首先进行对齐(暂时不做)

    int i = 0;
    for (; i + AVX_S <= size; i += AVX_S) {
        int mask = SIMD_bit_vector(SIMD_condition(x+i, left, right));
        if (mask != 0) {
            for (int j = 1; j <= AVX_S; ++j) {
                // 注意首先应该处理高位
                if (mask >> (AVX_S - j) & 1){
                    return *(y+i+j-1);
                }
            }
            
        }
    }
    for (; i < size; ++i) {
        if (left <= x[i] && x[i] <= right){
            return y[i];
        }
    }
    return NOT_EXSIST;
}

// 查看 x 数组中所有范围在 [left, right] 的 y 值列表
std::vector<int> find_all_matches_simd(int *x, int *y, int size, int left, int right);

int get_matches_sum_simd(int *x, int *y, int size, int left, int right);

int count_matches_simd(int *x, int size, int left, int right);

float get_matches_avg_simd(int *x, int *y, int size, int left, int right);

int get_matches_min(int *x, int *y, int size, int left, int right);


