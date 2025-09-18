//
// Created by 86180 on 25-9-17.
//
#include <iostream>
using namespace std;

int findMIN(string s) {
    int lenth = s.size();
    string s1;
    for (int i = lenth-1;i>=lenth/2;i--) {
        s1.push_back(s[i]);
        s.pop_back();
    }
    if (s==s1) {
        return findMIN(s);
    }else {
        return lenth;
    }
}

int main() {
    string s;
    cin>>s;
    int res = findMIN(s);
    cout<< res;
    return 0;
}