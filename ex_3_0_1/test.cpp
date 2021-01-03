#include"scan_like_operations.h"
#include<iostream>
#include<malloc.h>
#include<random>
#include<sys/time.h>
using namespace std;

static std::default_random_engine engine;
static std::uniform_int_distribution<unsigned> dis(0, 100000000);
const int len = 1;
static int cnt[] = {100000000};

void run_test_match();

static int find_first_match(int *x, int *y, int size, int left, int right) {
    for (int i = 0; i < size; ++i) {
        if (left <= x[i] && x[i] <= right) {
            return y[i];
        }
    }
    return -1;
}

static vector<int> find_all_matches(int *x, int *y, int size, int left, int right) {
    vector<int> res;
    for (int i = 0; i < size; ++i) {
        if (left <= x[i] && x[i] <= right) {
            res.push_back(y[i]);
        }
    }
    return res;
}

void test_first_match() {
    int x[] = {34, 65, 25, 26 , 27, 56, 78, 9, 1};
    int y[] = {34, 5, 25, 6 , 7, 56, 78, 99, 100};
    int size = 9;
    int left = 0;
    int right = 10;
    cout << find_first_match_simd(x, y, size, left, right);
}

void test_all_matches() {
    int x[] = {65, 5, 26 , 27, 56, 78, 9, 1};
    int y[] = {5, 25, 6 , 7, 56, 78, 99, 100};
    //int x[] = {1,0,1,0,1,0,1,0};
    //int y[] = {1,1,1,1,1,1,1,1};
    int size = 8;
    int left = 0;
    int right = 10;
    vector<int> res = find_all_matches_simd(x, y, size, left, right);
    for (int i = 0; i < res.size(); ++i) {
        cout << res[i] << " ";
    }
    cout << endl;
}

int main() {
    //test_all_matches();
    run_test_match();
}

void run_test_match() {
    for (int i = 0; i < len; ++i) {
        int N = cnt[i];
        cout << "N = " << N << endl;
        int *x = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
        int *y = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
        int left = 0, right = 2000000;
        // 生成随机测试数据
        for (int j = 0; j < N; ++j) {
            //x[j] = dis(engine);
            x[j] = dis(engine);
            y[j] = dis(engine);
            // first match baseline
        }
        cout << "finish creating." << endl;
        // timer start
        struct timeval start;
        struct timeval end;
        unsigned long diff;
        gettimeofday(&start, NULL);
        // baseline
        int res1 = find_first_match(x, y, N, left, right);
        // timer end
        gettimeofday(&end, NULL);
        diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        cout << "baseline first match: " << res1 << " cost: " << diff << endl;

        // first match simd
        gettimeofday(&start, NULL);
        // simd
        int res2 = find_first_match_simd(x, y, N, left, right);
        // timer end
        gettimeofday(&end, NULL);
        diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        cout << "simd first match: " << res2 << " cost: " << diff << endl;


        vector<int> vt1, vt2;
        // all matches baseline
        gettimeofday(&start, NULL);
        // baseline
        vt1 = find_all_matches(x, y, N, left, right);
        // timer end
        gettimeofday(&end, NULL);
        diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        cout << "baseline all matches: " << vt1.size() << " cost: " << diff << endl;

        // all matches simd
        gettimeofday(&start, NULL);
        // simd
        vt2 = find_all_matches_simd(x, y, N, left, right);
        // timer end
        gettimeofday(&end, NULL);
        diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
        cout << "simd all matches: " << vt2.size() << " cost: " << diff << endl;  
        free(x);
        free(y);
    }
}
