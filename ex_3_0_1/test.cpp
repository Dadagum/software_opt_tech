#include"scan_like_operations.h"
#include<iostream>
using namespace std;

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
    test_all_match();
}