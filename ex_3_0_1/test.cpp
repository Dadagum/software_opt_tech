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
std::uniform_int_distribution<unsigned> match_dis(0, 100000);
std::uniform_int_distribution<unsigned> sum_dis(0, 20);
std::uniform_int_distribution<unsigned> cnt_dis(0, 100000);
std::uniform_int_distribution<unsigned> min_dis(0, 100000);
std::uniform_int_distribution<unsigned> search_dis(0, 10000);

void run_test_match(int, int, int);
void run_sum_match(int, int, int);
void run_cnt(int, int, int);
void run_avg(int, int, int);
void run_find_min(int, int, int);
void run_search(int);
void verify_search(int);

// match 测试规模
int match_size[] = {1000000, 10000000, 100000000};
int fm_nums = 3;
// sum 测试规模
int sum_size[] = {100000, 1000000, 10000000};
int sum_nums = 3;
// search 测试规模
int key_size[] = {512, 1024, 2048};
int key_nums = 3;

// 测试全部
void test_all() {
    int N, L, R;
    // 1. 测试 match
    L = 10000; R = 15000;
    cout << "test match ..." << endl;
    for (int i = 0; i < fm_nums; ++i) {
        N = match_size[i];
        cout << "N = " << N << endl;
        run_test_match(N, L, R);
    }
    cout << endl;

    // 2. 测试 sum
    L = 10; R = 15;
    cout << "test sum ..." << endl;
    for (int i = 0; i < sum_nums; ++i) {
        N = sum_size[i];
        cout << "N = " << N << endl;
        run_sum_match(N, L, R);
    }
    cout << endl;

    // 3. 测试 cnt
    L = 10000; R = 15000;
    cout << "test cnt ..." << endl;
    for (int i = 0; i < fm_nums; ++i) {
        N = match_size[i];
        cout << "N = " << N << endl;
        run_cnt(N, L, R);
    }

    // 4. 测试 min
    cout << "test min:" << endl;
    L = 10000; R = 15000;
    for (int i = 0; i < fm_nums; ++i) {
        N = match_size[i];
        cout << "N = " << N << endl;
        run_find_min(N, L, R);
    }

    // 5. 测试search
    cout << "test search" << endl;
    for (int i = 0; i < key_nums; ++i) {
        N = key_size[i];
        cout << "N = " << N << endl;
        run_search(N);
    }
    cout << endl;
}



int main(int argc, char const *argv[])
{
    int N = 100000000;
    int L = 0;
    int R = 100000000;
    // if (argc == 2) {
    //     N = atoi(argv[1]);
    // }
    // if (argc == 4) {
    //     N = atoi(argv[1]);
    //     L = atoi(argv[2]);
    //     R = atoi(argv[3]);
    // }
    //run_search(N);
//test_all();
    // 5. 测试search
    cout << "test search" << endl;
    for (int i = 0; i < key_nums; ++i) {
        N = key_size[i];
        cout << "N = " << N << endl;
        run_search(N);
    }
    cout << endl;

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
    for (int i = 0; i < 1; ++i) {
        target = search_dis(engine);
        res1 = scalar_binary_search(x, N, target);
        res2 = seq_cmp_simd(x, N, target);
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
    unsigned long diff, res1 = 0, res2 = 0, res3 = 0, res4 = 0;
    int target = 5000;
    for (int i = 0; i < 10000; ++i) {
        target = search_dis(engine);
        
        gettimeofday(&start, NULL);
        scalar_binary_search(x, N, target);
        gettimeofday(&end, NULL);
        diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        res1 += diff;

        gettimeofday(&start, NULL);
        naive_binary_search_simd(x, N, target);
        gettimeofday(&end, NULL);
        diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        res2 += diff;

        gettimeofday(&start, NULL);
        seq_cmp_simd(x, N, target);
        gettimeofday(&end, NULL);
        diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        res3 += diff;

        gettimeofday(&start, NULL);
        hybrid_search(x, N, target);
        gettimeofday(&end, NULL);
        diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        res4 += diff;
    }
    cout << "baseline binary search cost: " << res1 << endl;
    cout << "naive binary search cost: " << res2 << endl;
    cout << "seq binary search cost: " << res3 << endl;
    cout << "hybrid binary search cost: " << res4 << endl;
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
    //cout << "finish creating." << endl;

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
    //cout << "finish creating." << endl;

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
    //cout << "finish creating." << endl;
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
    //cout << "finish creating." << endl;
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
    //cout << "finish creating." << endl;

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


