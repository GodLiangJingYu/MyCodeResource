//
// Created by 86180 on 25-9-16.
//
#include <cmath>
#include <iostream>
using namespace std;

bool huiwen(int m) {
    int val = m,res=0;
    while (val>0) {
        res*=10;
        res+=val%10;
        val=val/10;
    }
    if (res==m)
        return true;
    return false;
}

int defence(int leftVal,int rightVal) {
    int res = 0;
    int minVal = static_cast<int>(sqrt(leftVal));
    if (minVal*minVal<leftVal) {
        minVal++;
    }
    while (minVal*minVal<=rightVal) {
        if (huiwen(minVal)&&huiwen(minVal*minVal)) {
            res++;
        }
        minVal++;
    }
    return res;
}

int main() {
    int leftVal,rightVal;
    cin>>leftVal>>rightVal;
    int output = defence(leftVal,rightVal);
    cout<<output;
    return 0;
}