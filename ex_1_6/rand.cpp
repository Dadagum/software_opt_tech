#include"ex_1_6.h"
#include<random>
//#include<iostream>
/**
 * 为了编程方便，假设每一个月都是 30 天，并且生成的身份证号码都是由数字 1-9 组成
**/

static std::default_random_engine engine;
static std::uniform_int_distribution<unsigned> month_distribution(1,12);
static std::uniform_int_distribution<unsigned> year_distribution(1900, 2020);
static std::uniform_int_distribution<unsigned> day_distribution(1, 30);
static std::uniform_int_distribution<unsigned> one_digit(1, 9);
static std::uniform_int_distribution<unsigned> one_alp(1, 26);

static std::string months[13] = {"N", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
static std::string days[31] = {"N", "01", "02", "03", "04", "05", "06", "07", "08", "09", 
                                    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                                    "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"};

static void int_fill_char_array(int end, char* mem, int val);
static void rand_n_digit(char* mem, int n);
static void rand_n_alp(char* mem, int n);
static void rand_id_card(char* card);

void gen_one_person(person *p) {
    // 生成 person
    // id
    rand_id_card(p->id);
    // name
    rand_n_alp(p->name, 20);
    // address
    rand_n_alp(p->address, 60);
    // phone
    rand_n_digit(p->phone_num, 14);
}

static void rand_id_card(char* card) {

    // 前 6 位: 0 - 5
    for (int i = 0; i < 6; ++i) {
        card[i]  = '0' + one_digit(engine);
    }
    // 中间 8 位，身份证日期 : 6 - 13
    // 年: 6 - 9
    int_fill_char_array(9, card, year_distribution(engine));
    // 月: 10 - 11
    int month = month_distribution(engine);
    if (month < 10) {
        card[10] = '0';
        card[11] = '0' + month;
    } else {
        int_fill_char_array(11, card, month);
    }
    // 日: 12 - 13
    int day = day_distribution(engine);
    if (day < 10) {
        card[12] = '0';
        card[13] = '0' + day;
    } else {
        int_fill_char_array(13, card, day);
    }
    // 后面 4 位: 14 - 17
       for (int i = 14; i < 18; ++i) {
        card[i] = '0' + one_digit(engine);
    }

}

static void int_fill_char_array(int end, char* mem, int val) {
    while (val) {
        int d = val % 10;
        mem[end] = '0' + d;
        val /= 10;
        --end;
    }
}

static void rand_n_digit(char* mem, int n) {
    for (int i = 0; i < n; ++i){
        mem[i] = '0' + one_digit(engine);
    }
}

static void rand_n_alp(char* mem, int n) {
    for (int i = 0; i < n; ++i) {
        mem[i] = 'a' + one_alp(engine);
    }
}

// int main() {
//     person data[32];
//     for (int i = 0; i < 30; ++i) {
//         gen_one_person(&data[i]);
//         for (int j = 0; j < 18; ++j) std::cout << data[i].id[j];
//         std::cout << std::endl;
//     }
// }