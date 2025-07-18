//
// Created by 86180 on 25-7-16.
//
#include <iostream>
#include <string>
using namespace std;

bool isMatch(string s, string p){
    int snum = 0 , pnum = 0;
    while (snum < s.size() && pnum < p.size()) {

    }
}

int main() {
    string s, p;
    cout << "请输入字符串s:";
    cin >> s;
    cout << "请输入模式p:";
    cin >> p;
    bool result = isMatch(s, p);
    cout << "是否匹配: " << (result ? "true" : "false") << endl;
    return 0;
}
/*
 * .就是跳过该字符，*就是判断和前一个字符是否相同
 */