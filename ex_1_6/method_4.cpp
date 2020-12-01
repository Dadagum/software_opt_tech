#include<iostream>
#include<map>
#include<vector>
#include<string>
#include"ex_1_6.h"
using namespace std;

map<long long, int> mps[366];
person *persons;
//int cnt = 0;

// 假设每个月都是 30 天，传入 0203 这种格式的日期
int extract_days(char* str) {
    int m = (str[0] - '0') * 10 + (str[1] - '0');
    int d = (str[2] - '0') * 10 + (str[3] - '0');
    //cout << "extract m = " << m << " d = " << d << endl;
    return m == 1 ? d : (m-1) * 30 + d;
}

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
        int days = extract_days(&(p[i].id[10]));
        (mps[days])[key] = i;
    }
}

person *person_search(char *id) {
    long long key = encode(id, 18);
    int days = extract_days(&id[10]);
    auto iter = mps[days].find(key);
    //if (iter != mps[days].end()) cnt++;
    return iter == mps[days].end() ? nullptr : &(persons[iter->second]);
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
   // cout << "hit : " << cnt << endl;
    return 0;
}