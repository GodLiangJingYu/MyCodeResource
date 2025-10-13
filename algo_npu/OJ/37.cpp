//
// Created by 86180 on 25-10-11.
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// 反序列化输入字符串为二维向量
vector<vector<int>> deserialize(string str) {
    // 移除首尾的方括号
    str = str.substr(1, str.length() - 2);

    // 分割每个子数组
    vector<vector<int>> result;
    stringstream ss(str);
    string item;

    while (getline(ss, item, ']')) {
        // 清理字符串，去掉多余的字符
        if (item.empty()) continue;
        if (item[0] == ',') item = item.substr(1);
        if (item[0] == '[') item = item.substr(1);

        // 分割宽度和高度
        stringstream num_ss(item);
        string num;
        vector<int> rect(2);
        int idx = 0;
        while (getline(num_ss, num, ',')) {
            rect[idx++] = stoi(num);
        }
        result.push_back(rect);
    }

    return result;
}

// 计算最多能嵌套的长方形数量
int nestedRectangles(vector<vector<int>>& rectangles) {
    sort(rectangles.begin(), rectangles.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] == b[0] ? a[1] > b[1] : a[0] < b[0];
    });

    int n = rectangles.size();
    vector<int> dp(n, 1);
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (rectangles[j][0] < rectangles[i][0] && rectangles[j][1] < rectangles[i][1]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
    }
    return *max_element(dp.begin(), dp.end());
}

int main() {
    string input;
    getline(cin, input); // 读取输入字符串，如 "[[6,4],[7,3],[7,6],[1,2]]"

    vector<vector<int>> rectangles = deserialize(input);
    int res = nestedRectangles(rectangles);
    cout << res << endl;

    return 0;
}