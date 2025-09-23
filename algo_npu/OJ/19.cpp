//
// Created by 86180 on 25-9-19.
//
#include <iostream>
#include <queue>
using namespace std;

bool whoWin(int a,int b) {
    if (b==0) {
        return false;
    }
    if (a/b>=2) {
        return true;
    }
    return !whoWin(b,a%b);
}

int main() {
    int Ts;
    cin>>Ts;
    vector<bool> vecs;
    while (Ts--) {
        int s,o;
        cin>>s>>o;
        if (s>o) {
            vecs.push_back(whoWin(s,o));
        }else {
            vecs.push_back(whoWin(o,s));
        }
    }
    for (auto vec:vecs) {
        if (vec==true) {
            cout<<"Stan wins"<<endl;
        }else {
            cout<<"Ollie wins"<<endl;
        }
    }
    return 0;
}
