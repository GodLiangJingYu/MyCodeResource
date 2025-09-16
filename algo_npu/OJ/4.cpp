//
// Created by 86180 on 25-9-15.
//
#include <algorithm>
#include <iostream>
using namespace std;

long long solution(int n, int m, int t) {
    long long res=0;
    if (n<m&&n<t) {
        return 1;
    }
    int loop = n/m;
    for (int i=0;i<=loop;i++) {
        res += (n-i*m) /t + 1;
    }
    return res;
}



int main()
{
    int n, m, t;
    cin >> n >> m >> t;
    cout << solution(n, m, t) << endl;
    return 0;
}

