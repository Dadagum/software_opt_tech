#include"scan_like_operations.h"

int find_first_match_simd(int *x, int *y, int size, int left, int right) {
    // 首先进行对齐(暂时不做)
    __m256i LEFT = _mm256_set1_epi32(left-1);
    __m256i RIGHT = _mm256_set1_epi32(right+1);
    int i = 0;
    for (; i + AVX_S <= size; i += AVX_S) {
        __m256i X = _mm256_loadu_si256((__m256i*)(x+i));
        // SIMD_condition
        __m256i con = _mm256_and_si256(_mm256_cmpgt_epi32(RIGHT, X), _mm256_cmpgt_epi32(X, LEFT));
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

// sse：计算 4 通道相加的和 
inline 
int hsum_epi32_sse2(__m128i x) {
    __m128i hi64  = _mm_unpackhi_epi64(x, x);
    __m128i sum64 = _mm_add_epi32(hi64, x);
    __m128i hi32  = _mm_shufflelo_epi16(sum64, _MM_SHUFFLE(1, 0, 3, 2));
    __m128i sum32 = _mm_add_epi32(sum64, hi32);
    return _mm_cvtsi128_si32(sum32);
}

// 计算满足 x 范围在 [left, right] 中的所有 y 值的和
int get_matches_sum_simd(int *x, int *y, int size, int left, int right) {
    __m128i LEFT = _mm_set1_epi32(left-1);
    __m128i RIGHT = _mm_set1_epi32(right+1);
    __m128i v_ans = _mm_set1_epi32(0);
    __m128i X, Y, con;
    int i = 0;
    for (; i + SSE_S <= size; i += SSE_S) {
        X = _mm_loadu_si128((__m128i*)(x+i));
        Y = _mm_loadu_si128((__m128i*)(y+i));
        // SIMD_condition
        con = _mm_and_si128(_mm_cmpgt_epi32(RIGHT, X), _mm_cmpgt_epi32(X, LEFT));
        v_ans = _mm_add_epi32(v_ans, _mm_and_si128(con, Y));
    }
    int ans = hsum_epi32_sse2(v_ans);
    // 剩下部分
    for (; i < size; ++i) {
        ans += (left <= x[i] && x[i] <= right) ? y[i] : 0;
    }
    return ans;
}

// 计算满足 x 范围在 [left, right] 中的个数
int count_matches_simd(int *x, int size, int left, int right) {
    __m128i LEFT = _mm_set1_epi32(left-1);
    __m128i RIGHT = _mm_set1_epi32(right+1);
    __m128i v_ans = _mm_set1_epi32(0);
    __m128i X, con;
    int ans = 0;
    int i = 0;
    for (; i + SSE_S <= size; i += SSE_S) {
        X = _mm_loadu_si128((__m128i*)(x+i));
        // SIMD_condition
        con = _mm_and_si128(_mm_cmpgt_epi32(RIGHT, X), _mm_cmpgt_epi32(X, LEFT));
        ans += hsum_epi32_sse2(con);
    }
    // 剩下部分
    for (; i < size; ++i) {
        ans += (left <= x[i] && x[i] <= right) ? -1 : 0;
    }
    // 将加和结果直接取反就是最终的结果
    return -ans;
}

float get_matches_avg_simd(int *x, int *y, int size, int left, int right) {
    __m128i LEFT = _mm_set1_epi32(left-1);
    __m128i RIGHT = _mm_set1_epi32(right+1);
    __m128i v_ans = _mm_set1_epi32(0);
    __m128i X, Y, con;
    int i = 0;
    int cnt = 0;
    for (; i + SSE_S <= size; i += SSE_S) {
        X = _mm_loadu_si128((__m128i*)(x+i));
        Y = _mm_loadu_si128((__m128i*)(y+i));
        // SIMD_condition
        con = _mm_and_si128(_mm_cmpgt_epi32(RIGHT, X), _mm_cmpgt_epi32(X, LEFT));
        cnt += hsum_epi32_sse2(con);
        v_ans = _mm_add_epi32(v_ans, _mm_and_si128(con, Y));
    }
    int sum = hsum_epi32_sse2(v_ans);
    // 剩下部分
    for (; i < size; ++i) {
        sum += (left <= x[i] && x[i] <= right) ? y[i] : 0;
        cnt += (left <= x[i] && x[i] <= right) ? -1 : 0;
    }
    return cnt ? -1.0 * sum / cnt : 0;
}

int get_matches_min_simd(int *x, int *y, int size, int left, int right) {
    __m256i LEFT = _mm256_set1_epi32(left-1);
    __m256i RIGHT = _mm256_set1_epi32(right+1);
    __m256i INF = _mm256_set1_epi32(INT_MAX);
    __m256i v_min = INF;
    __m256i X, Y, con, tmp;
    int i = 0, s_min = INT_MAX;
    for (; i + AVX_S <= size; i += AVX_S) {
        X = _mm256_loadu_si256((__m256i*)(x+i));
        Y = _mm256_loadu_si256((__m256i*)(y+i));
        // simd condition
        con = _mm256_and_si256(_mm256_cmpgt_epi32(RIGHT, X), _mm256_cmpgt_epi32(X, LEFT));
        // [INF, y1, INF, y2 ...] 的形式
        tmp = _mm256_or_si256(_mm256_and_si256(con, Y), _mm256_andnot_si256(con, INF));
        // 更新 v_min
        con = _mm256_cmpgt_epi32(v_min, tmp);
        v_min = _mm256_blendv_epi8 (v_min, tmp, con);
    }
    // 最简单的查找最小方法：直接内存中找吧。应该可以用特殊的 simd 指令来加速这个过程
    int mem[8];
    _mm256_storeu_si256((__m256i*)mem, v_min);
    for (int i = 0; i < 8; ++i) {
        s_min = std::min(s_min, mem[i]);
    }
    for (; i < size; ++i) {
        if (left <= x[i] && x[i] <= right){
            s_min = std::min(s_min, y[i]);
        }
    }
    return s_min;
}


