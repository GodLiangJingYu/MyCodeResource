//
// Created by 86180 on 25-9-18.
//
#include <iostream>
#include <stack>
using namespace std;

string outputS(string s) {
    string res;
    stack<int> stack;
    for (int i = 0;i<s.size();i++) {
        if (s[i]=='[') {
            stack.push(i);
        }else if (s[i]==']') {
            if (stack.size()==1) {
                int l = stack.top();
                string nextS = s.substr(l+1, i - l - 1);
                int idx = 0;
                while (idx < (int)nextS.size() && nextS[idx] >= '0' && nextS[idx] <= '9') idx++;
                int repeat = 1;
                if (idx > 0) {
                    repeat = stoi(nextS.substr(0, idx));
                }
                string inner = nextS.substr(idx);
                s.erase(l, i - l + 1);
                string decodedInner = outputS(inner);
                string rep;
                rep.reserve(decodedInner.size() * (size_t)repeat);
                for (int t = 0; t < repeat; ++t) rep += decodedInner;

                s.insert(l, rep);
                i = l + (int)rep.size() - 1;
            }
            stack.pop();
        }
    }
    return s;
}

int main() {
    string s;
    if (!(cin >> s)) return 0;
    string res = outputS(s);
    cout << res;
    return 0;
}
