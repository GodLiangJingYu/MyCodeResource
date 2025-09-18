//
// Created by 86180 on 25-9-16.
//
#include <iostream>
using namespace std;

long long countval(int x) {
    if (x==1) {
        return 6;
    }
    return countval(x-1)*2+2;
}

int main() {
    int x;
    long long res;
    cin>>x;
    res = countval(x);
    cout<<res;
    return 0;
}