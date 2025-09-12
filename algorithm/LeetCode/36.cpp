//
// Created by 86180 on 25-9-10.
//
#include <iostream>
#include <unordered_map>
#include <vector>

// 你需要补全的 isValidSudoku 函数
bool isValidSudoku(const std::vector<std::vector<char>>& board) {
    // 行
    for (int i = 0; i < 9; i++) {
        std::vector<bool> hashmap(10, false);
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != '.') {
                int num = board[i][j] - '0';
                if (num < 1 || num > 9) return false;
                if (hashmap[num] == false) {
                    hashmap[num] = true;
                } else {
                    return false;
                }
            }
        }
    }
    // 列
    for (int j = 0; j < 9; j++) {
        std::vector<bool> hashmap2(10, false);
        for (int i = 0; i < 9; i++) {
            if (board[i][j] != '.') {
                int num = board[i][j] - '0';
                if (num < 1 || num > 9) return false;
                if (hashmap2[num] == false) {
                    hashmap2[num] = true;
                } else {
                    return false;
                }
            }
        }
    }
    // 小矩阵
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            std::vector<bool> hashmap3(10, false);
            for (int row = i; row < i + 3; row++) {
                for (int col = j; col < j + 3; col++) {
                    if (board[row][col] != '.') {
                        int num = board[row][col] - '0';
                        if (num < 1 || num > 9) return false;
                        if (hashmap3[num] == false) {
                            hashmap3[num] = true;
                        } else {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

// 辅助函数：打印数独板
void printBoard(const std::vector<std::vector<char>>& board) {
    for (const auto& row : board) {
        for (char c : row) {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }
}

// main 函数
int main() {
    // 示例 1
    std::vector<std::vector<char>> board1 = {
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '.', '7', '9'}
    };

    bool result1 = isValidSudoku(board1);
    std::cout << "示例 1 结果: " << (result1 ? "true" : "false") << std::endl;

    // 示例 2
    std::vector<std::vector<char>> board2 = {
        {'8', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '.', '7', '9'}
    };

    bool result2 = isValidSudoku(board2);
    std::cout << "示例 2 结果: " << (result2 ? "true" : "false") << std::endl;

    return 0;
}