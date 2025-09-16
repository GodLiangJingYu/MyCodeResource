//
// Created by 86180 on 25-9-15.
//
#include <iostream>
using namespace std;

int reverseNum(int x) {
    int rev = 0;
    while (x > 0) {
        rev = rev * 10 + x % 10;
        x /= 10;
    }
    return rev;
}

bool isExist(int num) {
    for (int m = max(0, num / 2); m <= num; ++m) {
        if (m + reverseNum(m) == num) {
            return true;
        }
    }
    return false;
}



int main()
{
    int num;
    cin >> num;
    if(isExist(num)){
        cout << "True" << endl;
    }
    else{
        cout << "False" << endl;
    }
    return 0;
}