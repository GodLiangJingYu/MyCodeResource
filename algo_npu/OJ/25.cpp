//
// Created by 86180 on 25-9-21.
//
#include <iostream>
#include <vector>
using namespace std;

long long N;
vector<long long> len;

void prepareLen(long long N) {
    len.push_back(3); // S(0) = "moo"
    int k = 0;
    while (len.back() < N) {
        k++;
        len.push_back(len.back() * 2 + k + 3);
    }
}

char solve(int k, long long pos) {
    if (k == 0) {
        if (pos == 1) return 'm';
        else return 'o';
    }
    long long left = len[k - 1];
    long long mid = k + 3;
    if (pos <= left) {
        return solve(k - 1, pos);
    } else if (pos > left && pos <= left + mid) {
        if (pos == left + 1) return 'm';
        else return 'o';
    } else {
        return solve(k - 1, pos - left - mid);
    }
}

int main() {
    cin >> N;
    prepareLen(N);
    int k = len.size() - 1;
    cout << solve(k, N);
    return 0;
}