//
// Created by 86180 on 25-9-23.
//
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

// 回溯法生成全排列
void backtrack(const vector<int>& nums, vector<int>& current, vector<bool>& used, vector<vector<int>>& result) {
    if (current.size() == nums.size()) {
        result.push_back(current);
        return;
    }
    for (size_t i = 0; i < nums.size(); ++i) {
        if (!used[i]) {
            used[i] = true;
            current.push_back(nums[i]);
            backtrack(nums, current, used, result);
            current.pop_back();
            used[i] = false;
        }
    }
}

// 全排列方法实现
vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> current;
    vector<bool> used(nums.size(), false);
    backtrack(nums, current, used, result);
    // 如果要按字典序排列
    sort(result.begin(), result.end());
    return result;
}

int main() {
    string line;
    getline(cin, line);
    stringstream ss(line);
    vector<int> nums;
    int x;
    while (ss >> x) {
        nums.push_back(x);
    }
    vector<vector<int>> result = permute(nums);
    for (const auto& permutation : result) {
        for (size_t i = 0; i < permutation.size(); ++i) {
            cout << permutation[i];
            if (i != permutation.size() - 1) cout << " ";
        }
        cout << endl;
    }
    return 0;
}