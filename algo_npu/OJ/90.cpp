#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int N = 16;

struct Grid {
    char board[N][N];
    vector<pair<int, int>> empty;
    
    void read() {
        for (int i = 0; i < N; i++) {
            string line;
            getline(cin, line);
            for (int j = 0; j < N; j++) {
                board[i][j] = line[j];
                if (board[i][j] == '-') {
                    empty.push_back({i, j});
                }
            }
        }
    }
    
    void print() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << board[i][j];
            }
            cout << endl;
        }
    }
    
    bool isValid(int row, int col, char ch) {
        // Check row
        for (int j = 0; j < N; j++) {
            if (board[row][j] == ch) return false;
        }
        
        // Check column
        for (int i = 0; i < N; i++) {
            if (board[i][col] == ch) return false;
        }
        
        // Check 4x4 box
        int boxRow = row / 4 * 4;
        int boxCol = col / 4 * 4;
        for (int i = boxRow; i < boxRow + 4; i++) {
            for (int j = boxCol; j < boxCol + 4; j++) {
                if (board[i][j] == ch) return false;
            }
        }
        
        return true;
    }
    
    bool solve(int idx) {
        if (idx == empty.size()) return true;
        
        int row = empty[idx].first;
        int col = empty[idx].second;
        
        for (char c = 'A'; c <= 'P'; c++) {
            if (isValid(row, col, c)) {
                board[row][col] = c;
                if (solve(idx + 1)) {
                    return true;
                }
                board[row][col] = '-';
            }
        }
        
        return false;
    }
};

int main() {
    int k;
    cin >> k;
    cin.ignore(); // Ignore newline
    
    while (k--) {
        Grid grid;
        
        // Read potential empty line before data
        if (k > 0) {
            string line;
            getline(cin, line);
            if (line.empty()) {
                grid.read();
            } else {
                // Line contains data, process it
                for (int j = 0; j < N; j++) {
                    grid.board[0][j] = line[j];
                    if (line[j] == '-') {
                        grid.empty.push_back({0, j});
                    }
                }
                for (int i = 1; i < N; i++) {
                    getline(cin, line);
                    for (int j = 0; j < N; j++) {
                        grid.board[i][j] = line[j];
                        if (line[j] == '-') {
                            grid.empty.push_back({i, j});
                        }
                    }
                }
            }
        } else {
            grid.read();
        }
        
        grid.solve(0);
        grid.print();
        
        if (k > 0) {
            cout << endl;
        }
    }
    
    return 0;
}