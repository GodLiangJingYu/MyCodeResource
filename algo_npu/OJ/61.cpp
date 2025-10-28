//
// Created by 86180 on 2025/10/27.
//
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 函数声明：生成杨辉三角的前 num 行
vector<vector<int>> generate(int num);

// 格式化输出函数，确保不输出 0
string formatOutput(const vector<vector<int>>& result) {
    string output = "[";
    for (size_t i = 0; i < result.size(); ++i) {
        output += "[";
        // 只输出第 i 行的前 i+1 个元素（从 0 到 i）
        for (size_t j = 0; j <= i; ++j) {
            output += to_string(result[i][j]);
            if (j < i) { // 最后一个有效元素后不加逗号
                output += ",";
            }
        }
        output += "]";
        if (i < result.size() - 1) {
            output += ",";
        }
    }
    output += "]";
    return output;
}

int main() {
    int num;
    cin >> num;
    vector<vector<int>> result = generate(num);
    cout << formatOutput(result) << endl;
    return 0;
}

vector<vector<int>> generate(int num) {
    // 初始化 n x n 数组，全部填 0
    vector<vector<int>> triangle(num, vector<int>(num, 0));

    if (num == 0) return triangle;

    // 生成杨辉三角
    for (int i = 0; i < num; ++i) {
        // 每行首尾元素为 1
        triangle[i][0] = 1; // 第一个元素
        if (i > 0) triangle[i][i] = 1; // 最后一个元素（第 i 行有 i+1 个元素）

        // 计算中间元素
        for (int j = 1; j < i; ++j) {
            // 公式：triangle[i][j] = triangle[i-1][j-1] + triangle[i-1][j]
            triangle[i][j] = triangle[i-1][j-1] + triangle[i-1][j];
        }
    }

    return triangle;
}