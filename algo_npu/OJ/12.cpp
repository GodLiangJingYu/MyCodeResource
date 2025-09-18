//
// Created by 86180 on 25-9-16.
//
#include <iostream>
using namespace std;

int main() {
    int num ,right;
    cin>>num;
    cin>>right;
    for (right;right>1;right--) {
        num /=10;
    }
    int res = num%10;
    cout<< res;
    return 0;
}