#pragma once

/**
 * 二分搜索，默认传入数组从小到大排序
 * 这里主要考虑 B+ 树中 key 的个数为 512，即数组长度都为 512
 **/

// 注意，这里是 upper_bound 的过程，因为需要找到 key 的索引
int scalar_binary_search(int *arr, int size, int target);

int naive_binary_search_simd(int *arr, int size, int target);

int seq_cmp_simd(int *arr, int size, int target);

int hybrid_search(int *arr, int size, int target);