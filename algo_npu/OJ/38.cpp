//
// Created by 86180 on 25-10-11.
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// 解析输入字符串为二维字符数组
vector<vector<char>> parseStringTo2DArray(string input) {
    if (input.length() >= 2 && input[0] == '{' && input[1] == '{') {
        input = input.substr(2, input.length() - 4);
    }
    vector<vector<char>> matrix;
    stringstream ss(input);
    string rowStr;
    while (getline(ss, rowStr, '}')) {
        if (rowStr.empty()) continue;
        if (rowStr[0] == '{') rowStr = rowStr.substr(1);
        vector<char> row;
        stringstream rowSs(rowStr);
        string cell;
        while (getline(rowSs, cell, ',')) {
            if (cell.empty()) continue;
            cell.erase(remove_if(cell.begin(), cell.end(), [](char c) {
                return isspace(c) || c == '\'' || c == '\"' || c == '{';
            }), cell.end());
            if (!cell.empty()) {
                row.push_back(cell[0]);
            }
        }
        if (!row.empty()) {
            matrix.push_back(row);
        }
    }
    return matrix;
}

// 计算最多能击杀的敌人数量
int maxKilledEnemies(vector<vector<char>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;

    int rows = grid.size();
    int cols = grid[0].size();

    // 辅助数组：rowHits[i][j] 表示在 (i,j) 向左和向右能击杀的敌人数量
    // colHits[i][j] 表示在 (i,j) 向上和向下能击杀的敌人数量
    vector<vector<int>> rowHits(rows, vector<int>(cols, 0));
    vector<vector<int>> colHits(rows, vector<int>(cols, 0));

    // 预计算每行的敌人数量
    for (int i = 0; i < rows; ++i) {
        // 从左到右
        int count = 0;
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 'B') {
                count = 0; // 遇到障碍重置计数
            } else if (grid[i][j] == 'E') {
                count++; // 遇到敌人增加计数
            }
            rowHits[i][j] += count; // 记录向左的敌人数量
        }
        // 从右到左
        count = 0;
        for (int j = cols - 1; j >= 0; --j) {
            if (grid[i][j] == 'B') {
                count = 0;
            } else if (grid[i][j] == 'E') {
                count++;
            }
            rowHits[i][j] += count; // 累加向右的敌人数量
        }
    }

    // 预计算每列的敌人数量
    for (int j = 0; j < cols; ++j) {
        // 从上到下
        int count = 0;
        for (int i = 0; i < rows; ++i) {
            if (grid[i][j] == 'B') {
                count = 0;
            } else if (grid[i][j] == 'E') {
                count++;
            }
            colHits[i][j] += count; // 记录向上的敌人数量
        }
        // 从下到上
        count = 0;
        for (int i = rows - 1; i >= 0; --i) {
            if (grid[i][j] == 'B') {
                count = 0;
            } else if (grid[i][j] == 'E') {
                count++;
            }
            colHits[i][j] += count; // 累加向下的敌人数量
        }
    }

    // 遍历所有 'S' 位置，计算最大击杀数
    int maxEnemies = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 'S') {
                int totalEnemies = rowHits[i][j] + colHits[i][j];
                maxEnemies = max(maxEnemies, totalEnemies);
            }
        }
    }

    return maxEnemies;
}

int main() {
    string input;
    getline(cin, input);
    vector<vector<char>> grid = parseStringTo2DArray(input);
    int result = maxKilledEnemies(grid);
    cout << result << endl;
    return 0;
}