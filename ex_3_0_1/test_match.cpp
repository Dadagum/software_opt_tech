#include"scan_like_operations.h"
#include<iostream>
#include<malloc.h>
#include<random>
#include<sys/time.h>
using namespace std;

static std::default_random_engine engine;
static std::uniform_int_distribution<unsigned> dis(1, 1e6);
const int len = 3;
static int cnt[] = {1000000, 10000000, 100000000};

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

void test_all_match() {
    int x[] = {34, 65, 5, 26 , 27, 56, 78, 9, 1};
    int y[] = {34, 5, 25, 6 , 7, 56, 78, 99, 100};
    int size = 9;
    int left = 0;
    int right = 10;
    vector<int> res = find_all_matches_simd(x, y, size, left, right);
    for (int i = 0; i < res.size(); ++i) {
        cout << res[i] << " ";
    }
}


int main() {
    for (int i = 0; i < len; ++i) {
        int N = cnt[i];
        cout << "N = " << N << endl;
        int *x = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
        int *y = (int*) memalign(AVX_ALIGN, sizeof(int)*N);
        int left = 5000, right = 5500;
        for (int j = 0; j < N; ++j) {
            x[j] = dis(engine);
            y[j] = dis(engine);
            // first match baseline
	}
            // timer start
	     struct timeval start;
            struct timeval end;
            unsigned long diff;
            gettimeofday(&start, NULL);
            // exec multply
            int res1 = find_first_match(x, y, N, left, right);
            // timer end
            gettimeofday(&end, NULL);
            diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
            cout << "baseline first match: " << res1 << " cost: " << diff << endl;

            // first match simd
            gettimeofday(&start, NULL);
            // exec multply
            int res2 = find_first_match_simd(x, y, N, left, right);
            // timer end
            gettimeofday(&end, NULL);
            diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
            cout << "simd first match: " << res2 << " cost: " << diff << endl;

            // all matches baseline
            gettimeofday(&start, NULL);
            // exec multply
            res1 = find_all_matches(x, y, N, left, right).size();
            // timer end
            gettimeofday(&end, NULL);
            diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
            cout << "baseline all matches: " << res1 << " cost: " << diff << endl;

            // all matches simd
            gettimeofday(&start, NULL);
            // exec multply
            res2 = find_all_matches_simd(x, y, N, left, right).size();
            // timer end
            gettimeofday(&end, NULL);
            diff =  1e6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
            cout << "simd all matches: " << res2 << " cost: " << diff << endl;
        
	free(x);
	free(y);
    }
    
}

