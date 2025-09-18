//
// Created by 86180 on 25-9-17.
//
#include <iostream>
using namespace std;

int partition(int m, int n) {
    if (m == 0 || n == 1)
        return 1;
    if (n == 0 || m < 0)
        return 0;
    return partition(m, n - 1) + partition(m - n, n);
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        int m, n;
        cin >> m >> n;
        cout << partition(m, n) << endl;
    }
    return 0;
}