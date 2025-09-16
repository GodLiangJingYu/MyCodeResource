//
// Created by 86180 on 25-9-14.
//
#include <iostream>
#include <stack>
using namespace std;

stack<char> Back(string s) {
    stack<char> res;
    for (auto c : s) {
        if (c == '#'&&!res.empty()) {
            res.pop();
        }else if (c != '#'){
            res.push(c);
        }
    }
    return res;
}

bool isEqual(string S, string T) {
    stack<char> Ss = Back(S);
    stack<char> Ts = Back(T);
    while (!Ss.empty()&&!Ts.empty()) {
        if (Ss.top()!=Ts.top()) {
            return false;
        }
        Ss.pop();
        Ts.pop();
    }
    return Ss.empty() && Ts.empty();
}



int main()
{
    string s1;
    string s2;
    cin >> s1 >> s2;
    if(isEqual(s1, s2)){
        cout << "True" << endl;
    }
    else{
        cout << "False" << endl;
    }
    return 0;
}