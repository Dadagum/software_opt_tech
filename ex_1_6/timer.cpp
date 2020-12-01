#include<sys/time.h>
#include<iostream>
#include"ex_1_6.h"


void print_insert_n_time(void (*func)(person*, int), int N, person *p) {
    struct timeval start;
    struct timeval end;
    unsigned long diff;
    // timer start
    gettimeofday(&start, NULL);
    // start inserting
    func(p, N);
    // timer end
    gettimeofday(&end, NULL);
    diff = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    double insert_per_sec =  N * 1e6 / diff ;
    std::cout << "total insert time: " << diff << std::endl;
    std::cout << "insert per sec: " << insert_per_sec << std::endl;
}

void print_search_n_time(person* (*func)(char*), int N, person *p) {
    struct timeval start;
    struct timeval end;
    unsigned long diff;
    // timer start
    gettimeofday(&start, NULL);
    // start searching
    for (int i = 0; i < 1e6; ++i) {
        func(p[zero_to_n(N)].id);
    }
    // timer end
    gettimeofday(&end, NULL);
    diff = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    double search_per_sec =  N * 1e6 / diff ;
    std::cout << "total search time: " << diff << std::endl;
    std::cout << "search per sec: " << search_per_sec << std::endl;
}

