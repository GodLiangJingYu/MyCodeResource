//
// Created by 86180 on 2025/10/27.
//
#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    map<pair<int, int>, double> memo; // 使用 map 存储状态，避免大数组

    // 递归计算概率
    double dfs(int a, int b) {
        // 终止条件
        if (a <= 0 && b <= 0) return 0.5; // 同时分配完
        if (a <= 0) return 1.0; // 鸡汤先分配完
        if (b <= 0) return 0.0; // 排骨汤先分配完

        // 检查是否已计算
        if (memo.count({a, b})) return memo[{a, b}];

        // 四种操作，每种概率 0.25
        double prob = 0.25 * (
            dfs(max(0, a - 100), b) +              // 100ml 鸡汤
            dfs(max(0, a - 75), max(0, b - 25)) +  // 75ml 鸡汤, 25ml 排骨汤
            dfs(max(0, a - 50), max(0, b - 50)) +  // 50ml 鸡汤, 50ml 排骨汤
            dfs(max(0, a - 25), max(0, b - 75))    // 25ml 鸡汤, 75ml 排骨汤
        );

        memo[{a, b}] = prob;
        return prob;
    }

public:
    double soupServings(int n) {
        // 阈值优化：当 n 较大时，概率接近 1
        if (n >= 5000) return 1.0;
        return dfs(n, n);
    }
};

int main() {
    int n;
    cin >> n;
    Solution solution;
    cout << fixed << setprecision(5) << solution.soupServings(n) << endl;
    return 0;
}