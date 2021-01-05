#include"scan_like_operations.h"
#include<iostream>
#include<malloc.h>
#include<random>
#include<sys/time.h>
using namespace std;

std::default_random_engine engine(time(0));
std::uniform_int_distribution<unsigned> match_dis(0, 100000000);
std::uniform_int_distribution<unsigned> sum_dis(0, 100);
std::uniform_int_distribution<unsigned> cnt_dis(0, 20000000);
std::uniform_int_distribution<unsigned> min_dis(0, 20000000);
// 默认
int N = 20000000;
int L = 5000;
int R = 10000;

void run_test_match();
void run_sum_match();
void run_cnt();
void run_avg();
void run_find_min();

int find_min(int *x, int *y, int size, int left, int right) {
    int ans = INT_MAX;
    for (int i = 0; i < size; ++i) {
        if (left <= x[i] && x[i] <= right) {
            ans = std::min(ans, y[i]);
        }
    }
    return ans;
}

float cal_avg(int *x, int *y, int size, int left, int right) {
    int sum = 0;
    int cnt = 0;
    for (int i = 0; i < size; ++i) {
        sum += (left <= x[i] && x[i] <= right) ? y[i] : 0;
        cnt += (left <= x[i] && x[i] <= right) ? -1 : 0;
    }
    return cnt ? -1.0 * sum / cnt : 0;

}

int cnt_match(int *x, int size, int left, int right) {
    int ans = 0;
    for (int i = 0; i < size; ++i) {
        ans += (left <= x[i] && x[i] <= right) ? 1 : 0;
    }
    return ans;
}

int cal_sum(int *x, int *y, int size, int left, int right) {
    int ans = 0;
    for (int i = 0; i < size; ++i) {
        ans += (left <= x[i] && x[i] <= right) ? y[i] : 0;
    }
    return ans;
}

int find_first_match(int *x, int *y, int size, int left, int right) {
    for (int i = 0; i < size; ++i) {
        if (left <= x[i] && x[i] <= right) {
            return y[i];
        }
    }
    return -1;
}

vector<int> find_all_matches(int *x, int *y, int size, int left, int right) {
    vector<int> res;
    for (int i = 0; i < size; ++i) {
        if (left <= x[i] && x[i] <= right) {
            res.push_back(y[i]);
        }
    }
    return res;
}

int main(int argc, char const *argv[])
{
    if (argc == 4) {
        N = atoi(argv[1]);
        L = atoi(argv[2]);
        R = atoi(argv[3]);
    }
    /* code */
    run_find_min();
    return 0;
}

void run_find_min() {
    int *x = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
    int *y = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
    // 生成随机测试数据
    for (int j = 0; j < N; ++j) {
        //x[j] = dis(engine);
        x[j] = min_dis(engine);
        y[j] = min_dis(engine);
    }
    cout << "finish creating." << endl;

    // timer start
    struct timeval start;
    struct timeval end;
    unsigned long diff;
    gettimeofday(&start, NULL);
    // baseline
    float res1 = find_min(x, y, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "baseline min: " << res1 << " cost: " << diff << endl;
    gettimeofday(&start, NULL);
    // simd
    float res2 = get_matches_min_simd(x, y, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "simd min: " << res2 << " cost: " << diff << endl;
    free(x);
    free(y);
}

void run_avg() {
    int *x = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
    int *y = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
    // 生成随机测试数据
    for (int j = 0; j < N; ++j) {
        //x[j] = dis(engine);
        x[j] = sum_dis(engine);
        y[j] = sum_dis(engine);
    }
    cout << "finish creating." << endl;

    // timer start
    struct timeval start;
    struct timeval end;
    unsigned long diff;
    gettimeofday(&start, NULL);
    // baseline
    float res1 = cal_avg(x, y, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "baseline avg: " << res1 << " cost: " << diff << endl;
    gettimeofday(&start, NULL);
    // simd
    float res2 = get_matches_avg_simd(x, y, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "simd avg: " << res2 << " cost: " << diff << endl;
    free(x);
    free(y);
}

void run_cnt() {
    int *x = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
    // 生成随机测试数据
    for (int j = 0; j < N; ++j) {
        x[j] = cnt_dis(engine);
    }
    cout << "finish creating." << endl;
    // timer start
    struct timeval start;
    struct timeval end;
    unsigned long diff;
    gettimeofday(&start, NULL);
    // baseline
    int res1 = cnt_match(x, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "baseline cnt: " << res1 << " cost: " << diff << endl;

    gettimeofday(&start, NULL);
    // simd
    int res2 = count_matches_simd(x, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "simd cnt: " << res2 << " cost: " << diff << endl;
    free(x);
}


void run_test_match() {
    int *x = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
    int *y = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
    // 生成随机测试数据
    for (int j = 0; j < N; ++j) {
        x[j] = match_dis(engine);
        y[j] = match_dis(engine);
    }
    cout << "finish creating." << endl;
    // timer start
    struct timeval start;
    struct timeval end;
    unsigned long diff;
    gettimeofday(&start, NULL);
    // baseline
    int res1 = find_first_match(x, y, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "baseline first match: " << res1 << " cost: " << diff << endl;

    // first match simd
    gettimeofday(&start, NULL);
    // simd
    int res2 = find_first_match_simd(x, y, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "simd first match: " << res2 << " cost: " << diff << endl;


    vector<int> vt1, vt2;
    // all matches baseline
    gettimeofday(&start, NULL);
    // baseline
    vt1 = find_all_matches(x, y, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "baseline all matches: " << vt1.size() << " cost: " << diff << endl;

    // all matches simd
    gettimeofday(&start, NULL);
    // simd
    vt2 = find_all_matches_simd(x, y, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "simd all matches: " << vt2.size() << " cost: " << diff << endl;  
    free(x);
    free(y);
    
}

void run_sum_match() {
    int *x = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
    int *y = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
    // 生成随机测试数据
    for (int j = 0; j < N; ++j) {
        x[j] = sum_dis(engine);
        y[j] = sum_dis(engine);
    }
    cout << "finish creating." << endl;

    // timer start
    struct timeval start;
    struct timeval end;
    unsigned long diff;
    gettimeofday(&start, NULL);
    // baseline
    int res1 = cal_sum(x, y, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "baseline sum: " << res1 << " cost: " << diff << endl;

    gettimeofday(&start, NULL);
    // simd
    int res2 = get_matches_sum_simd(x, y, N, L, R);
    // timer end
    gettimeofday(&end, NULL);
    diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    cout << "simd sum: " << res2 << " cost: " << diff << endl;
    free(x);
    free(y);
}


