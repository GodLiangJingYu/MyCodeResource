//
// Created by 86180 on 25-9-25.
//
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

// 旋转单个字符的规则
char rotateChar(char c) {
    if (c == '0') return '0';
    if (c == '1') return '1';
    if (c == '8') return '8';
    if (c == '6') return '9';
    if (c == '9') return '6';
    return '\0'; // 无效字符
}

vector<string> find180RotationNum(int a) {
    vector<string> result;
    if (a == 1) {
        result = {"0", "1", "8"};
        return result;
    }
    int half = (a + 1) / 2;
    string s(a, ' ');
    function<void(int)> dfs = [&](int pos) {
        if (pos == half) {
            for (int i = 0; i < half; ++i) {
                char rotated = rotateChar(s[i]);
                if (rotated == '\0') return;
                s[a - 1 - i] = rotated;
            }
            result.push_back(s);
            return;
        }
        string choices;
        if (pos == 0) {
            choices = "1689";
        } else if ((a % 2 == 1) && (pos == half - 1)) {
            choices = "018";
        } else {
            choices = "01689";
        }
        for (char c : choices) {
            s[pos] = c;
            dfs(pos + 1);
        }
    };
    dfs(0);
    sort(result.begin(), result.end());
    return result;
}

int main() {
    int a;
    cin >> a;
    vector<string> result = find180RotationNum(a);
    for (size_t i = 0; i < result.size(); ++i) {
        if (i > 0) cout << " ";
        cout << result[i];
    }
    cout << endl;
    return 0;
}