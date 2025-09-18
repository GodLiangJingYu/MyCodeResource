//
// Created by 86180 on 25-9-16.
//
#include <iostream>
#include <string>
using namespace std;

char findChar(const string& s, long long n, long long len) {
    if (n <= s.size()) return s[n-1];
    long long half = len / 2;
    if (n <= half) {
        return findChar(s, n, half);
    } else {
        long long pos = n - half;
        if (pos == 1) {
            return findChar(s, half, half);
        } else {
            return findChar(s, pos - 1, half);
        }
    }
}

int main() {
    string s;
    long long N;
    if (!(cin >> s >> N)) return 0;
    long long len = s.size();
    while (len < N) {
        len *=2;
    }
    cout << findChar(s, N, len) ;
    return 0;
}