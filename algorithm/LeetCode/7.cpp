//
// Created by 86180 on 25-7-12.
//
#include <iostream>
#include <queue>
using namespace std;

// 你自己实现这个函数
int reverse(int x) {
    long MIN = -2147483648L, MAX = 2147483647L;
    long res = 0;
    queue<int> q;
    while (x != 0) {
        q.push(x % 10);
        x /= 10;
    }
    while (!q.empty()) {
        res *= 10;
        res += q.front();
        q.pop();
    }
    if (res > MAX || res < MIN) {
        return 0; // 溢出处理
    }
    return res;
}

int main() {
    int x;
    cout << "input " << endl;
    cin >> x;
    cout <<x<<endl;
    int result = reverse(x);
    cout << "output: " << result << endl;
    return 0;
}
