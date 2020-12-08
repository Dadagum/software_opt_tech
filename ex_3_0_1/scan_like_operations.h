#pragma once
#include<vector>
/**
 * 模拟数据库行为：对所有 x 范围在 [left, right] 的元组，对 y 进行相应的处理
 * 使用 simd 加快行为
**/

// 查看 x 数组中第一个范围在 [left, right] 的 y 值
int find_first_match_simd(int *x, int *y, int size, int left, int right);

// 查看 x 数组中所有范围在 [left, right] 的 y 值列表
std::vector<int> find_all_matches_simd(int *x, int *y, int size, int left, int right);

int get_matches_sum_simd(int *x, int *y, int size, int left, int right);

int count_matches_simd(int *x, int size, int left, int right);

float get_matches_avg_simd(int *x, int *y, int size, int left, int right);

int get_matches_min(int *x, int *y, int size, int left, int right);