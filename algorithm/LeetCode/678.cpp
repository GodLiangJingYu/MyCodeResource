//
// Created by 86180 on 25-9-12.
//
#include <iostream>
#include <stack>
#include <string>

// 你需要实现的函数
bool checkValidString(std::string s) {
    std::stack<int> stackkuo,stackxing;
    for (int i=0;i<s.size();i++) {
        if (s[i]=='(') {
            stackkuo.push(i);
        }else if (s[i]=='*') {
            stackxing.push(i);
        }else {
            if (stackkuo.empty()) {
                if (stackxing.empty()) {
                    return false;
                }else {
                    stackxing.pop();
                }
            }else {
                stackkuo.pop();
            }
        }
    }
    if (stackkuo.empty()) {
        return true;
    }else {
        while (!stackkuo.empty() && !stackxing.empty()) {
            if (stackkuo.top()<=stackxing.top()) {
                stackkuo.pop();
                stackxing.pop();
            }else {
                return false;
            }
        }
        if (!stackkuo.empty()) {
            return false;
        }
        return true;
    }
}

int main() {
    std::string input;
    std::cout << "input:";
    std::cin >> input;

    if (checkValidString(input)) {
        std::cout << "true\n";
    } else {
        std::cout << "false\n";
    }

    return 0;
}