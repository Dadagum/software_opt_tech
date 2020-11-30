#include<iostream>
#include"ex2_3.h"
using namespace std;

static bool init_zero = false;
static int num_of_prefix_zero[SIZE];

static void init_zero_table() {
    cout << "init table..." << endl;
    for (int i = 0; i < SIZE; ++i) {
        int cnt = 0;
        for (int j = i; j; j >>= 1) 
            ++cnt;
        num_of_prefix_zero[i] = 16 - cnt;
    }
     cout << "init end ..." << endl;
}

int cnt_prefix_zero(unsigned x) {
    if (!init_zero) {
        init_zero_table();
        init_zero = true;
    }
    if (x < SIZE) {
        return 16 + num_of_prefix_zero[x];
    }
    int hi_x = (x >> 16) & MASK;
    return num_of_prefix_zero[hi_x];
}


int main() {
    unsigned x;
    while (cin >> x) {
        int ans = cnt_prefix_zero(x);
        cout << "# of zero = " << ans << endl;
    }
    return 0;
}