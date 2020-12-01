#pragma once

struct person {
    char id[18];
    char name[20];
    char address[60];
    char phone_num[14];
};

// method_x.cpp
void person_insert(person *p, int N);
person *person_search(char *id);

// rand.cpp
void gen_one_person(person *p);
void gen_n_person(person *p, int N);
unsigned zero_to_n(int N);

// timer.cpp
void print_search_n_time(person* (*func)(char*), int N, person *p);
void print_insert_n_time(void (*func)(person*, int), int N, person *p);


