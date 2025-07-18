//
// Created by 86180 on 25-7-17.
//
#include <iostream>
using namespace std;

static constexpr string R[4][10] {
    {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"}, // 个位
    {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"}, // 十位
    {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"}, // 百位
    {"", "M", "MM", "MMM"},
};

string intToRoman(int num) {
    return R[3][num / 1000] + R[2][num / 100 % 10] + R[1][num / 10 % 10] + R[0][num % 10];
}

int main() {
    int num;
    cout << "输入:";
    cin >> num;
    string result = intToRoman(num);
    cout << "罗马字符: " << result << endl;
    return 0;
}