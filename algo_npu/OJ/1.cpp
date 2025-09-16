//
// Created by 86180 on 25-9-14.
//
#include <iostream>
using namespace std;

int solution(string s)
{
    if (s.empty()) {
        return 0;
    }
    int res=0;
    for (int i = s.size()-1 ; i>=0 ; --i) {
        if (s[i]!=' ') {
            res++;
        }else {
            break;
        }
    }
    return res;
}

int main()
{
    string str;
    getline(cin, str);
    cout << solution(str) << endl;
    return 0;
}