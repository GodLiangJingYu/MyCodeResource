//
// Created by 86180 on 25-9-19.
//
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int cnt = 1;
vector<vector<int>> board;

void tile(int n, int x, int y, int special_x, int special_y) {
    if (n == 1) return;
    int half = n / 2;
    int t = cnt++;
    int sx[4] = {x, x, x + half, x + half};
    int sy[4] = {y, y + half, y, y + half};
    int idx = (special_x < x + half ? 0 : 2) + (special_y < y + half ? 0 : 1);

    pair<int,int> child_special[4];
    for (int d = 0; d < 4; ++d) {
        if (d == idx) {
            child_special[d] = {special_x, special_y};
        } else {
            int px = sx[d] + (d >= 2 ? 0 : half - 1);
            int py = sy[d] + (d % 2 == 1 ? 0 : half - 1);
            board[px][py] = t;
            child_special[d] = {px, py};
        }
    }
    for (int d = 0; d < 4; ++d) {
        tile(half, sx[d], sy[d], child_special[d].first, child_special[d].second);
    }
}

int main() {
    int k, p, q;
    if (!(cin >> k >> p >> q)) return 0;
    int n = 1 << k;
    board.assign(n, vector<int>(n, 0));
    tile(n, 0, 0, p - 1, q - 1);

    unordered_map<int,int> remap;
    int nextId = 1;
    // 行优先扫描，严格按照样例分配编号
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int v = board[i][j];
            if (v == 0) continue;
            if (!remap.count(v)) remap[v] = nextId++;
        }
    }
    // 再填回棋盘
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (board[i][j] != 0) board[i][j] = remap[board[i][j]];
        }
    }
    // 输出
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cout << board[i][j] << (j == n-1 ? '\n' : ' ');
    return 0;
}