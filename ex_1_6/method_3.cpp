#include<iostream>
#include<map>
#include<vector>
#include<string>
#include"ex_1_6.h"
using namespace std;

map<long long, int> mp;
person *persons;
//int cnt = 0;

long long encode(char* str, int n) {
    long long val = 0;
    for (int i = 0; i < n; ++i) {
        val += str[i] - '0';
        val *= 10;
    }
    return val;
}

void person_insert(person *p, int N) {
    for (int i = 0; i < N; ++i) {
        long long key = encode(p[i].id, 18);
        mp[key] = i;
    }
}

person *person_search(char *id) {
    long long key = encode(id, 18);
    auto iter = mp.find(key);
    //if (iter != mp.end()) cnt++;
    return iter == mp.end() ? nullptr : &(persons[iter->second]);
}

int main() {
    int N;
    cout << "enter N: " << endl;
    cin >> N;
    persons = new person[N];
    gen_n_person(persons, N);
    cout << "finish gen" << endl;
    // insert
    print_insert_n_time(person_insert, N, persons);
    // search
    print_search_n_time(person_search, N, persons);
    delete[] persons;
    //cout << "hit : " << cnt << endl;
    return 0;
}