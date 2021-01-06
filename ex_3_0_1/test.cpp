#include"scan_like_operations.h"
#include<iostream>
#include<malloc.h>
#include<random>
#include<sys/time.h>
#include<limits.h>
#include<algorithm>
#include"search.h"
#include"c.h"
using namespace std;

std::default_random_engine engine(time(0));
std::uniform_int_distribution<unsigned> match_dis(0, 100000000);
std::uniform_int_distribution<unsigned> sum_dis(0, 100);
std::uniform_int_distribution<unsigned> cnt_dis(0, 20000000);
std::uniform_int_distribution<unsigned> min_dis(0, 20000000);
std::uniform_int_distribution<unsigned> search_dis(0, 10000);

void run_test_match(int, int, int);
void run_sum_match(int, int, int);
void run_cnt(int, int, int);
void run_avg(int, int, int);
void run_find_min(int, int, int);
void run_search(int);
void verify_search(int);

int main(int argc, char const *argv[])
{
    int N = 100000000;
    int L = 0;
    int R = 100000000;
    if (argc == 2) {
        N = atoi(argv[1]);
    }
    if (argc == 4) {
        N = atoi(argv[1]);
        L = atoi(argv[2]);
        R = atoi(argv[3]);
    }
    //run_search(N);
    verify_search(N);
    return 0;
}

void verify_search(int N) {
    //N = 16;
    int *x = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
    //int x[] = {0, 76 , 345 , 470 , 474 , 534 , 605 , 668, 726};
    
    // 生成随机测试数据
    for (int j = 0; j < N; ++j) {
        x[j] = search_dis(engine);
        //x[j] = j*3 + 1;
    }
    sort(x, x+N);
    // for (int j = 0; j < N; ++j) {
    //     cout << x[j] << " ";
    // }
    // cout << endl;
    int res1, res2, target;
    target = 330;
    for (int i = 0; i < 10000; ++i) {
        target = search_dis(engine);
        res1 = scalar_binary_search(x, N, target);
        res2 = hybrid_search(x, N, target);
        if (res1 != res2) {
            for (int j = 0; j < N; ++j) {
                cout << x[j] << " ";
            }
            cout << endl;
            cout << "miss match: target = " << target << "  baseline = " << res1 << " simd = " << res2 << endl;
        }
    }
    free(x);
}

// N = 512
void run_search(int N) {
    int *x = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
    // 生成随机测试数据
    for (int j = 0; j < N; ++j) {
        //x[j] = dis(engine);
        x[j] = search_dis(engine);
    }
    std::sort(x, x+N);
    // timer start
    struct timeval start;
    struct timeval end;
    unsigned long diff, res1 = 0, res2 = 0;
    int target = 5000;
    for (int i = 0; i < 10000; ++i) {
        target = search_dis(engine);
        
        gettimeofday(&start, NULL);
        scalar_binary_search(x, N, target);
        gettimeofday(&end, NULL);
        diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        res1 += diff;

        gettimeofday(&start, NULL);
        hybrid_search(x, N, target);
        gettimeofday(&end, NULL);
        diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        res2 += diff;
    }
    cout << "baseline binary search cost: " << res1 << endl;
    cout << "simd binary search cost: " << res2 << endl;
    free(x);
}

void run_find_min(int N, int L, int R) {
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

void run_avg(int N, int L, int R) {
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

void run_cnt(int N, int L, int R) {
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

void run_test_match(int N, int L, int R) {
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

void run_sum_match(int N, int L, int R) {
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


