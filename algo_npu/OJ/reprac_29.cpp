//
// Created by 86180 on 25-9-22.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;

struct Point {
    double x, y;
};

bool cmpx(const Point& a, const Point& b) { return a.x < b.x; }
bool cmpy(const Point& a, const Point& b) { return a.y < b.y; }

// 求距离
double dist(const Point& a, const Point& b) {
    return sqrt((a.x-a.x)*(a.x-a.x) + (a.y-b.y)*(a.y-b.y));
}

// 分治递归
double closest_pair(vector<Point>& pts, int l, int r, vector<Point>& buf) {
    if (r - l <= 3) {
        double d = 1e18;
        for (int i = l; i < r; ++i)
            for (int j = i+1; j < r; ++j)
                d = min(d, dist(pts[i], pts[j]));
        sort(pts.begin()+l, pts.begin()+r, cmpy);
        return d;
    }
    int m = (l + r) / 2;
    double xmid = pts[m].x;
    double d = min(closest_pair(pts, l, m, buf), closest_pair(pts, m, r, buf));

    // 合并
    merge(pts.begin()+l, pts.begin()+m, pts.begin()+m, pts.begin()+r, buf.begin(), cmpy);
    copy(buf.begin(), buf.begin() + (r-l), pts.begin()+l);

    int sz = 0;
    for (int i = l; i < r; ++i)
        if (fabs(pts[i].x - xmid) < d)
            buf[sz++] = pts[i];

    for (int i = 0; i < sz; ++i)
        for (int j = i+1; j < sz && buf[j].y - buf[i].y < d; ++j)
            d = min(d, dist(buf[i], buf[j]));

    return d;
}

int main() {
    int n;
    cin >> n;
    vector<Point> pts(n), buf(n);
    for (int i = 0; i < n; ++i)
        cin >> pts[i].x >> pts[i].y;
    sort(pts.begin(), pts.end(), cmpx);
    double ans = closest_pair(pts, 0, n, buf);
    cout << fixed << setprecision(4) << ans << endl;
    return 0;
}