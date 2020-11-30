#include<iostream>
#include"ex2_3.h"
using namespace std;

static int num_of_one[SIZE];
static bool init_one = false;

static void init_one_table() {
    cout << "init table..." << endl;
    for (int i = 0; i < SIZE; ++i) {
        int cnt = 0;
        for (int j = i; j; j >>= 1) 
            cnt += j & 1;
        num_of_one[i] = cnt;
    }
    cout << "init end ..." << endl;
}

int cnt_one(unsigned x) {
    if (!init_one) {
        init_one_table();
        init_one = true;
    }
    int cnt = 0;
    int lo_x = x & MASK;
    int hi_x = (x >> 16) & MASK;
    cnt = num_of_one[lo_x] + num_of_one[hi_x];
    return cnt;
}




int main() {
    unsigned x;
    while (cin >> x) {
        int ans = cnt_one(x);
        cout << "# of ones = " << ans << endl;
    }
    return 0;
}