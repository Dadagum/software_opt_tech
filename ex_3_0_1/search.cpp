#include<nmmintrin.h>
#include<immintrin.h>
#include"search.h"
#include"c.h"
#define L_LEN 64

// 后续 naive_binary_search_simd 函数可能会复用该代码
inline int binary_search_helper(int *arr, int size, int target, int left, int right) {
    while (left < right) {
        int mid = (left + right) / 2;
        if (arr[mid] > target) right = mid;
        else left = mid + 1;
    }
    return left;
}

// 注意，这里是 upper_bound 的过程, 因为需要找到 key 的索引
int scalar_binary_search(int *arr, int size, int target) {
    return binary_search_helper(arr, size, target, 0, size);
}

// 实测,该版本和 baseline 相差无几
int naive_binary_search_simd(int *arr, int size, int target) {
    int cnt = size / AVX_S; 
    int left = 0, right = cnt;
    __m256i v_target = _mm256_set1_epi32(target);
    __m256i v_mid, v_con;
    while (left < right) {
        int mid = (left + right) / 2;
        v_mid = _mm256_loadu_si256((__m256i*)(arr + mid * AVX_S));
        v_con = _mm256_cmpgt_epi32(v_mid, v_target);
        int mask = _mm256_movemask_epi8(v_con);
        if (mask == -1) {
            // v_target 比 v_mid 都小，考虑当前和左边的情况
            // 注意有可能答案就是在这里
            // 例如 target = 6, v_mid = [7 .......], v_mid - 1 = [....... 5]
            // 细节：right 指向的要不是 cnt+1, 要不就是 mask = 0 的向量, 因此跳出循环即可返回, 不必验证答案是否在向量中间
            right = mid;
        } else if (mask == 0) {
            // 说明 v_target 中全部都大于 v_mid, 应该考虑右边情况
            left = mid + 1;
        } else {
            // 就在此部分
            for (int j = 0; j < 8; ++j) {
                if (mask & 1) return mid * AVX_S  + j;
                mask >>= 4;
            }
            // 不会返回
            return NOT_EXSIST;
        }
    }
    //return left * AVX_S;
    // 计算可能剩下的部分
    return binary_search_helper(arr, size - left * AVX_S, target, left * AVX_S, size);
}

// 利用 simd 从头比较到尾, 效果很差
int seq_cmp_simd(int *arr, int size, int target) {
    __m256i v_target = _mm256_set1_epi32(target);
    __m256i v_con, v_cur;
    int i;
    for (i = 0; i + AVX_S <= size; i += AVX_S) {
        v_cur = _mm256_loadu_si256((__m256i*)(arr +i));
        v_con = _mm256_cmpgt_epi32(v_cur, v_target);
        int mask = _mm256_movemask_epi8(v_con);
        if (mask != 0) {
            // 就在此部分
            for (int j = 0; j < 8; ++j) {
                if (mask & 1) return i + j;
                mask >>= 4;
            }
            // 不会返回
            return NOT_EXSIST;
        }
    }
    return binary_search_helper(arr, size - i, target, i, size);
}

// 结合 binary search 和 seq_cmp
int hybrid_search(int *arr, int size, int target) {
    int cnt = size / L_LEN; 
    int left = 0, right = cnt;
    while (left < right) {
        int mid = (left + right) / 2;
        if (arr[mid * L_LEN + L_LEN - 1] <= target) left = mid+1;
        else right = mid;
    }
    __m256i v_target = _mm256_set1_epi32(target);
    __m256i v_con, v_cur;
    for (int i = left * L_LEN; i + AVX_S <= size; i += AVX_S) {
        v_cur = _mm256_loadu_si256((__m256i*)(arr +i));
        v_con = _mm256_cmpgt_epi32(v_cur, v_target);
        int mask = _mm256_movemask_epi8(v_con);
        if (mask != 0) {
            // 就在此部分
            for (int j = 0; j < 8; ++j) {
                if (mask & 1) return i + j;
                mask >>= 4;
            }
            // 不会返回
            return NOT_EXSIST;
        }
    }
    return binary_search_helper(arr, size - left * L_LEN, target, left * L_LEN, size);
}