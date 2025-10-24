//
// Created by 86180 on 25-10-20.
//
#include <bits/stdc++.h>
using namespace std;

string largestNumber(vector<int>& cost, int target) {
    // dp[j] 表示成本为 j 时能组成的最大编号
    vector<string> dp(target + 1, "0");
    dp[0] = ""; // 成本为 0 时，编号为空字符串

    // 遍历所有可能的成本
    for (int j = 1; j <= target; ++j) {
        // 遍历所有零件编号（1 到 9）
        for (int i = 9; i >= 1; --i) {
            int idx = i - 1; // cost 数组索引
            if (j >= cost[idx] && dp[j - cost[idx]] != "0") {
                // 尝试用编号 i
                string curr = to_string(i) + dp[j - cost[idx]];
                // 更新 dp[j]，取较大的编号
                if (dp[j] == "0" || curr.length() > dp[j].length() ||
                    (curr.length() == dp[j].length() && curr > dp[j])) {
                    dp[j] = curr;
                    }
            }
        }
    }

    return dp[target];
}

int main() {
    vector<int> cost(9);
    for (int i = 0; i < 9; ++i) {
        cin >> cost[i];
    }
    int target;
    cin >> target;
    cout << largestNumber(cost, target) << endl;
    return 0;
}