#include"scan_like_operations.h"
#include<iostream>
using namespace std;


int main() {
    int x[] = {34, 23, 5, 6 , 7, 56, 78, 99};
    int y[] = {34, 23, 5, 6 , 7, 56, 78, 99};
    int size = 8;
    int left = 0;
    int right = 10;
    cout << find_first_match_simd(x, t, size, left, right);
}