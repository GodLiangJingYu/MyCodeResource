//
// Created by 86180 on 25-9-19.
//
#include <bits/stdc++.h>
using namespace std;

int tile_id = 1;

void place_tile(vector<vector<int>>& board, int n, int x, int y, int hx, int hy) {
    if (n == 1) {
        // 2x2 网格的基本情况
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                if (x + i != hx || y + j != hy) {
                    board[x + i][y + j] = tile_id;
                }
            }
        }
        tile_id++;
        return;
    }

    int size = 1 << n;
    int half = size >> 1;

    // 确定总部在哪个象限
    int quadrant;
    if (hx < x + half && hy < y + half) quadrant = 0;      // 左上
    else if (hx < x + half && hy >= y + half) quadrant = 1; // 右上
    else if (hx >= x + half && hy < y + half) quadrant = 2; // 左下
    else quadrant = 3;                                      // 右下

    // 中心位置的四个点
    int center_x = x + half - 1;
    int center_y = y + half - 1;

    // 放置L型骨牌（除了包含总部的象限）
    if (quadrant != 0) board[center_x][center_y] = tile_id;
    if (quadrant != 1) board[center_x][center_y + 1] = tile_id;
    if (quadrant != 2) board[center_x + 1][center_y] = tile_id;
    if (quadrant != 3) board[center_x + 1][center_y + 1] = tile_id;

    int current_id = tile_id++;

    // 递归处理四个象限
    if (quadrant == 0) {
        place_tile(board, n - 1, x, y, hx, hy);
        place_tile(board, n - 1, x, y + half, center_x, center_y + 1);
        place_tile(board, n - 1, x + half, y, center_x + 1, center_y);
        place_tile(board, n - 1, x + half, y + half, center_x + 1, center_y + 1);
    } else if (quadrant == 1) {
        place_tile(board, n - 1, x, y, center_x, center_y);
        place_tile(board, n - 1, x, y + half, hx, hy);
        place_tile(board, n - 1, x + half, y, center_x + 1, center_y);
        place_tile(board, n - 1, x + half, y + half, center_x + 1, center_y + 1);
    } else if (quadrant == 2) {
        place_tile(board, n - 1, x, y, center_x, center_y);
        place_tile(board, n - 1, x, y + half, center_x, center_y + 1);
        place_tile(board, n - 1, x + half, y, hx, hy);
        place_tile(board, n - 1, x + half, y + half, center_x + 1, center_y + 1);
    } else {
        place_tile(board, n - 1, x, y, center_x, center_y);
        place_tile(board, n - 1, x, y + half, center_x, center_y + 1);
        place_tile(board, n - 1, x + half, y, center_x + 1, center_y);
        place_tile(board, n - 1, x + half, y + half, hx, hy);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k, p, q;
    cin >> k >> p >> q;

    int n = 1 << k;
    vector<vector<int>> board(n, vector<int>(n, 0));

    // 调整坐标为0-based
    p--;
    q--;
    board[p][q] = 0;  // 总部位置

    tile_id = 1;
    place_tile(board, k, 0, 0, p, q);

    // 重新编号
    vector<int> new_id(n * n + 1, 0);
    int current_new_id = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int val = board[i][j];
            if (val == 0) {
                cout << 0;
            } else {
                if (new_id[val] == 0) {
                    new_id[val] = current_new_id++;
                }
                cout << new_id[val];
            }
            if (j < n - 1) cout << " ";
        }
      if (i != n-1) cout << '\n';
    }

    return 0;
}