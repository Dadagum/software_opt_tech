#pragma once
#include<vector>
/**
 * 模拟数据库行为：对所有 x 范围在 [left, right] 的元组，对 y 进行相应的处理
**/

// baseline 版本 
int find_min(int *x, int *y, int size, int left, int right);
float cal_avg(int *x, int *y, int size, int left, int right);
int cnt_match(int *x, int size, int left, int right);
int cal_sum(int *x, int *y, int size, int left, int right);
int find_first_match(int *x, int *y, int size, int left, int right);
std::vector<int> find_all_matches(int *x, int *y, int size, int left, int right);

// simd 版本
int find_first_match_simd(int *x, int *y, int size, int left, int right);
std::vector<int> find_all_matches_simd(int *x, int *y, int size, int left, int right);
int get_matches_sum_simd(int *x, int *y, int size, int left, int right);
int count_matches_simd(int *x, int size, int left, int right);
float get_matches_avg_simd(int *x, int *y, int size, int left, int right);
int get_matches_min_simd(int *x, int *y, int size, int left, int right);