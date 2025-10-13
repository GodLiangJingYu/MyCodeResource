//
// Created by 86180 on 25-9-19.
//
#include <bits/stdc++.h>
using namespace std;

int n, x, y;  // n = 2^k, x = p-1 (0-based row), y = q-1 (0-based col)
vector<vector<int>> matrix;

// 递归填充矩阵
void fill(int r1, int c1, int r2, int c2, int& num) {
    if (r1 > r2 || c1 > c2) return;  // 无效区域
    if (r1 == r2 && c1 == c2) {  // 1x1 区域
        if (r1 == x && c1 == y) {
            matrix[r1][c1] = 0;  // 总部
        } else {
            matrix[r1][c1] = num++;  // 填当前编号并递增
        }
        return;
    }

    int mid_r = (r1 + r2) / 2;  // 行中点
    int mid_c = (c1 + c2) / 2;  // 列中点
    int sub_size = (mid_r - r1 + 1) * (mid_c - c1 + 1);  // 子区域大小 m^2

    // 四子象限：TL (左上), BL (左下), TR (右上), BR (右下)
    // TL: r1..mid_r, c1..mid_c
    if (!(x >= r1 && x <= mid_r && y >= c1 && y <= mid_c)) {
        num += sub_size;  // 不含总部，跳过 m^2 个编号
    } else {
        fill(r1, c1, mid_r, mid_c, num);  // 递归进入
    }

    // BL: mid_r+1..r2, c1..mid_c
    if (!(x >= mid_r + 1 && x <= r2 && y >= c1 && y <= mid_c)) {
        num += sub_size;
    } else {
        fill(mid_r + 1, c1, r2, mid_c, num);
    }

    // TR: r1..mid_r, mid_c+1..c2
    if (!(x >= r1 && x <= mid_r && y >= mid_c + 1 && y <= c2)) {
        num += sub_size;
    } else {
        fill(r1, mid_c + 1, mid_r, c2, num);
    }

    // BR: mid_r+1..r2, mid_c+1..c2
    if (!(x >= mid_r + 1 && x <= r2 && y >= mid_c + 1 && y <= c2)) {
        num += sub_size;
    } else {
        fill(mid_r + 1, mid_c + 1, r2, c2, num);
    }
}

int main() {
    int k;
    scanf("%d%d%d", &k, &x, &y);
    x--;  // 转为 0-based
    y--;  // 转为 0-based
    n = 1 << k;  // 2^k
    matrix.assign(n, vector<int>(n, 0));  // 初始化矩阵
    int num = 1;  // 编号从 1 开始
    fill(0, 0, n - 1, n - 1, num);  // 填充整个矩阵

    // 输出矩阵
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j > 0) printf(" ");
            printf("%d", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}