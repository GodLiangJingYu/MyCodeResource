//
// Created by 86180 on 2025/10/27.
//
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <climits>

using namespace std;

int tallestBillboard(vector<int>& rods) {
    // dp[d] 表示高度差为 d 时，较小支架的最大高度
    unordered_map<int, int> dp;
    dp[0] = 0; // 初始状态：差值为 0，高度为 0

    for (int rod : rods) {
        // 复制当前状态
        unordered_map<int, int> curr(dp);
        for (auto& [d, height] : curr) {
            // 选项 1：不使用当前钢筋
            dp[d] = max(dp[d], height);

            // 选项 2：加到左支架（差值增加 rod）
            int newDiff = d + rod;
            dp[newDiff] = max(dp[newDiff], height + min(rod, abs(newDiff)));

            // 选项 3：加到右支架（差值减少 rod）
            newDiff = d - rod;
            dp[newDiff] = max(dp[newDiff], height + min(rod, abs(newDiff)));
        }
    }

    // 返回差值为 0 时的较小支架高度
    return dp.count(0) ? dp[0] : 0;
}

int main() {
    // 读取输入
    string input;
    getline(cin, input); // 读取一行输入
    stringstream ss(input);
    vector<int> rods;
    string token;

    // 按逗号分隔输入并转换为整数
    while (getline(ss, token, ',')) {
        rods.push_back(stoi(token));
    }

    // 调用函数并输出结果
    cout << tallestBillboard(rods) << endl;

    return 0;
}