#include"scan_like_operations.h"
#include<iostream>
using namespace std;


int main() {
    int x[] = {34, 65, 25, 26 , 7, 56, 78, 99, 1};
    int y[] = {34, 5, 25, 6 , 7, 56, 78, 99, 100};
    int size = 9;
    int left = 0;
    int right = 10;
    cout << find_first_match_simd(x, y, size, left, right);
}