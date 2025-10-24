//
// Created by 86180 on 25-9-23.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <climits>
#include <limits>

using namespace std;

struct Point {
    long long x, y;

    Point(long long x = 0, long long y = 0) : x(x), y(y) {}
};

// Calculate squared distance between two points
long long dist_sq(const Point& a, const Point& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

// Calculate perimeter squared for comparison purposes (not exact, but we can use it for sorting and nearest neighbors)
// Actually, we need the real distance for the perimeter
double dist_real(const Point& a, const Point& b) {
    return sqrt((double)(a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Comparator for sorting points by x, then by y
bool compare_x(const Point& a, const Point& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

// Find the minimum perimeter among triangles formed by points in a strip
double min_perimeter_strip(vector<Point>& strip, double d) {
    int n = strip.size();
    sort(strip.begin(), strip.end(), [](const Point& a, const Point& b) {
        return a.y < b.y;
    });

    double min_perim = numeric_limits<double>::max();

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n && (strip[j].y - strip[i].y) <= d; ++j) {
            double d1 = dist_real(strip[i], strip[j]);
            if (d1 >= d) continue;

            for (int k = j + 1; k < n && (strip[k].y - strip[i].y) <= d; ++k) {
                double d2 = dist_real(strip[i], strip[k]);
                double d3 = dist_real(strip[j], strip[k]);

                if (d2 >= d || d3 >= d) continue;

                double perim = d1 + d2 + d3;
                if (perim < min_perim) {
                    min_perim = perim;
                }
            }
        }
    }

    return min_perim;
}

double min_triangle_perimeter(vector<Point>& points, int left, int right) {
    if (right - left + 1 < 3) {
        return numeric_limits<double>::max();
    }
    if (right - left + 1 == 3) {
        Point a = points[left], b = points[left + 1], c = points[left + 2];
        double d1 = dist_real(a, b);
        double d2 = dist_real(b, c);
        double d3 = dist_real(a, c);
        return d1 + d2 + d3;
    }

    int mid = (left + right) / 2;
    Point mid_point = points[mid];

    double dl = min_triangle_perimeter(points, left, mid);
    double dr = min_triangle_perimeter(points, mid + 1, right);

    double d = min(dl, dr);

    // Collect points in the strip around the dividing line
    vector<Point> strip;
    for (int i = left; i <= right; ++i) {
        if (abs(points[i].x - mid_point.x) <= d) {
            strip.push_back(points[i]);
        }
    }

    double strip_min = min_perimeter_strip(strip, d);

    return min(d, strip_min);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int m;
    cin >> m;

    vector<Point> points(m);
    for (int i = 0; i < m; ++i) {
        cin >> points[i].x >> points[i].y;
    }

    sort(points.begin(), points.end(), compare_x);

    double result = min_triangle_perimeter(points, 0, m - 1);

    cout << fixed << setprecision(6) << result << "\n";

    return 0;
}