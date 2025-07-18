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
        char ch = s.front();
        s.pop_back();
        if (ch == 'C') {

        }else if (ch == 'X') {

        }else if (ch == 'I') {

        }
        res += ROMAN[ch];
    }
    return res;
}

int main() {
    cout <<"s =";
    string s;
    cin >> s ;
    cout << "s :" << endl ;
    int res = romanToInt(s);
    cout << res << endl;
    return 0;
}