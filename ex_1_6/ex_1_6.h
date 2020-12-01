#pragma once


struct person {
    char id[18];
    char name[20];
    char address[60];
    char phone_num[14];
};

void person_insert(person *p, int N);
person *person_search(char *id);

void gen_one_person(person *p);