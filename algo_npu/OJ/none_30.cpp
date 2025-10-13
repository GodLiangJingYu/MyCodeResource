//
// Created by 86180 on 25-9-23.
//
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

const int MAXN = 200005;
const double INF = 1e9;
const double EPS = 1e-10; // 浮点精度
double res = INF; // 全局最小周长
struct Point {
    long long x, y; // 使用 long long 防溢出
} p[MAXN];
int n;

// 计算两点距离
double dist(Point a, Point b) {
    return sqrt(1.0 * (a.x - b.x) * (a.x - b.x) + 1.0 * (a.y - b.y) * (a.y - b.y));
}

// 按 x 坐标排序
bool cmp(Point a, Point b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

// 暴力枚举三角形周长
double brute_force(int l, int r) {
    double min_perimeter = INF;
    for (int i = l; i <= r; i++) {
        for (int j = i + 1; j <= r; j++) {
            for (int k = j + 1; k <= r; k++) {
                double a = dist(p[i], p[j]);
                double b = dist(p[j], p[k]);
                double c = dist(p[k], p[i]);
                // 提前剪枝
                if (a > res + EPS || b > res + EPS || c > res + EPS) continue;
                // 允许共线（退化三角形）
                if (a + b >= c - EPS && b + c >= a - EPS && c + a >= b - EPS) {
                    min_perimeter = min(min_perimeter, a + b + c);
                }
            }
        }
    }
    return min_perimeter;
}

// 分治计算最近点对距离
double closest_pair(int l, int r) {
    if (r - l <= 0) return INF;
    if (r - l == 1) return dist(p[l], p[r]);

    int mid = (l + r) / 2;
    double mid_x = (p[mid].x + p[mid + 1].x) / 2.0;
    double d = min(closest_pair(l, mid), closest_pair(mid + 1, r));

    vector<Point> strip;
    for (int i = l; i <= r; i++) {
        if (abs(p[i].x - mid_x) < d + EPS) strip.push_back(p[i]);
    }
    sort(strip.begin(), strip.end(), [](Point a, Point b) { return a.y < b.y; });

    for (size_t i = 0; i < strip.size(); i++) {
        for (size_t j = i + 1; j < strip.size() && strip[j].y - strip[i].y < d + EPS; j++) {
            d = min(d, dist(strip[i], strip[j]));
        }
    }
    return d;
}

// 计算最小三角形周长
double solve(int l, int r) {
    if (r - l + 1 < 3) return INF;

    // 小规模直接暴力枚举
    if (r - l + 1 <= 20) {
        return brute_force(l, r);
    }

    // 分治
    int mid = (l + r) / 2;
    double mid_x = (p[mid].x + p[mid + 1].x) / 2.0;
    double min_perimeter = min(solve(l, mid), solve(mid + 1, r));

    // 收集中线附近点
    vector<Point> strip;
    for (int i = l; i <= r; i++) {
        if (abs(p[i].x - mid_x) < min_perimeter / 2 + EPS) strip.push_back(p[i]);
    }
    sort(strip.begin(), strip.end(), [](Point a, Point b) { return a.y < b.y; });

    // 枚举中线附近三角形
    for (size_t i = 0; i < strip.size(); i++) {
        for (size_t j = i + 1; j < strip.size() && strip[j].y - strip[i].y < min_perimeter / 2 + EPS; j++) {
            for (size_t k = j + 1; k < strip.size() && strip[k].y - strip[j].y < min_perimeter / 2 + EPS; k++) {
                double a = dist(strip[i], strip[j]);
                double b = dist(strip[j], strip[k]);
                double c = dist(strip[k], strip[i]);
                if (a > min_perimeter + EPS || b > min_perimeter + EPS || c > min_perimeter + EPS) continue;
                if (a + b >= c - EPS && b + c >= a - EPS && c + a >= b - EPS) {
                    min_perimeter = min(min_perimeter, a + b + c);
                }
            }
        }
    }

    return min_perimeter;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%lld%lld", &p[i].x, &p[i].y);
    }

    // 按 x 坐标排序
    sort(p, p + n, cmp);

    // 特殊处理小规模
    if (n <= 20) {
        res = brute_force(0, n - 1);
    } else {
        // 计算最近点对距离（共线情况）
        double min_dist = closest_pair(0, n - 1);
        res = min_dist == INF ? INF : 2 * min_dist;

        // 分治求解三角形周长
        double tri_perimeter = solve(0, n - 1);
        res = min(res, tri_perimeter);
    }

    // 避免输出 INF
    if (res == INF) res = 0; // 若无有效三角形，返回 0（视题目要求）
    printf("%.6lf\n", res);
    return 0;
}