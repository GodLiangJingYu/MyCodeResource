//
// Created by 86180 on 25-7-16.
//
#include <iostream>
#include <stack>
using namespace std;

bool isPalindrome(int x) {
    int num = x;
    stack<int> stack;
    if (num < 0) {
        return false;
    }
    while (num > 0) {
        stack.push(num % 10);
        num = num / 10;
    }
    while (!stack.empty()) {
        if (stack.top() != (x % 10)) {
            return false;
        }
        stack.pop();
        x = x / 10;
    }
    return true;
}

int main() {
    int x;
    cout << "in:";
    cin >> x;
    bool result = isPalindrome(x);
    cout << "y/N: " << (result ? "true" : "false") << endl;
    return 0;
}