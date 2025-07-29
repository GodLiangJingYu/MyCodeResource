//
// Created by 86180 on 25-7-17.
//
#include<iostream>
#include <unordered_map>
using namespace std;

unordered_map<char, int> ROMAN = {
    {'I', 1},
    {'V', 5},
    {'X', 10},
    {'L', 50},
    {'C', 100},
    {'D', 500},
    {'M', 1000},
};

int romanToInt(string s) {
    int res = 0;
    while (!s.empty()) {
        char ch = s.back();
        if (!s.empty()) {
            s.pop_back();
        }
        if (ch == 'D' && s.back() == 'C') {
            res += 400;
            s.pop_back();
        }else if (ch == 'M' && s.back() == 'C') {
            res += 900;
            s.pop_back();
        }else if (ch == 'L' && s.back() == 'X') {
            res += 40;
            s.pop_back();
        }else if (ch == 'C' && s.back() == 'X') {
            res += 90;
            s.pop_back();
        }else if (ch == 'V' && s.back() == 'I') {
            res += 4;
            s.pop_back();
        }else if (ch == 'X' && s.back() == 'I') {
            res += 9;
            s.pop_back();
        }else {
            res += ROMAN[ch];
        }
    }
    return res;
}

int main() {
    cout <<"s =";
    string s;
    cin >> s ;
    cout << "s :" << s << endl ;
    int res = romanToInt(s);
    cout << res << endl;
    return 0;
}