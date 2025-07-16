//
// Created by 86180 on 25-7-15.
//
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 这里声明 convert 函数，具体实现你自己补全
string convert(string s, int numRows){
    string res;
    int T = 2 * numRows - 2 , vec = 0;
    vector<string> vectors(numRows);
    if (numRows <= 1 || numRows >= s.size()) {
        return s; // 如果行数小于等于1或大于等于字符串长度，直接返回原字符串
    }
    for (int i = 0 ; i < s.size() ; i++) {
        if (i % T < numRows - 1) {
            vectors[vec++].push_back(s[i]);
        }else {
            vectors[vec--].push_back(s[i]);
        }
    }
    for (string vector : vectors) {
        res += vector;
    }
    return res;
}

int main() {
    string s;
    int numRows;
    cout << "s:";
    cin >> s;
    cout << "rows:";
    cin >> numRows;
    cout << convert(s, numRows) << endl;
    return 0;
}