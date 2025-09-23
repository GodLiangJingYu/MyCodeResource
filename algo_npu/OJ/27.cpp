//
// Created by 86180 on 25-9-22.
//
#include <iostream>
using namespace std;

long long xor1toN(long long n) {
    if (n % 4 == 0) return n;
    if (n % 4 == 1) return 1;
    if (n % 4 == 2) return n + 1;
    return 0;
}

int main() {
    long long N;
    cin >> N;
    cout << xor1toN(N) << endl;
    return 0;
}