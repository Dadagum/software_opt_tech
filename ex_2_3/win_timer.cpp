#include<sys/time.h>
#include<iostream>
using namespace std;

void bar() {
    for (int i = 0; i < 1e7; ++i);
}

unsigned longget_time() {
    struct timeval start;
    struct timeval end;
    unsigned long diff;
    gettimeofday(&start, NULL);
    bar(); // xxx
    gettimeofday(&end, NULL);
    diff = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    return diff;
}

int main() {
    cout << longget_time();
    return 0;
}