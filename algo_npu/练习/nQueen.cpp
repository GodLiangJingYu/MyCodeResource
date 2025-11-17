#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> result;
        vector<string> board(n, string(n, '.'));
        backtrack(0,board,result,n);
        return result;
    }
    
private:
    void backtrack(int row, vector<string>& board, vector<vector<string>>& result, int n) {
        if (row >= n) {
            result.push_back(board);
            return;
        }
        for (int col = 0; col < n; col++) {
            if (isValid(row, col, board, n)) {
                board[row][col] = 'Q';
                backtrack(row + 1, board, result, n);
                board[row][col] = '.';
            }
        }
    }
    
    bool isValid(int row, int col, vector<string>& board, int n) {
        // 检查同一列是否有其他皇后
        for (int i = 0; i < row; i++) {
            if (board[i][col] == 'Q') {
                return false;
            }
        }
        
        // 检查左上对角线是否有其他皇后
        for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
            if (board[i][j] == 'Q') {
                return false;
            }
        }
        
        // 检查右上对角线是否有其他皇后
        for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
            if (board[i][j] == 'Q') {
                return false;
            }
        }
        
        return true;
    }
};

// 测试代码
int main() {
    Solution solution;
    
    // 测试示例1
    int n1 = 4;
    vector<vector<string>> result1 = solution.solveNQueens(n1);
    cout << "n = " << n1 << " 的解法：" << endl;
    for (const auto& solution : result1) {
        for (const string& row : solution) {
            cout << row << endl;
        }
        cout << endl;
    }
    
    // 测试示例2
    int n2 = 1;
    vector<vector<string>> result2 = solution.solveNQueens(n2);
    cout << "n = " << n2 << " 的解法：" << endl;
    for (const auto& solution : result2) {
        for (const string& row : solution) {
            cout << row << endl;
        }
        cout << endl;
    }
    
    return 0;
}