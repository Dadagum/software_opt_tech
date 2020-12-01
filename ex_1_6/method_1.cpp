#include<iostream>
#include<map>
#include<vector>
#include<string>
#include"ex_1_6.h"
using namespace std;

map<string, person> mp;
//int cnt = 0;

void person_insert(person *p, int N) {
    for (int i = 0; i < N; ++i) {
        string key(p[i].id, p[i].id + 18);
        //cout << "insert key = " << key << endl;
        mp[key] = p[i];
    }
}

person *person_search(char *id) {
    string key(id, id + 18);
    auto iter = mp.find(key);
    //if (iter != mp.end()) cnt++;
    return iter == mp.end() ? nullptr : &(iter->second);
}

int main() {
    int N;
    cout << "enter N: " << endl;
    cin >> N;
    person *p = new person[N];
    gen_n_person(p, N);
    cout << "finish gen" << endl;
    // insert
    print_insert_n_time(person_insert, N, p);
    // search
    print_search_n_time(person_search, N, p);
    delete[] p;
    //cout << "hit : " << cnt << endl;
    return 0;
}