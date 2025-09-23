//
// Created by 86180 on 25-9-19.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

vector<double> res;

double jisuan(double x, double a, double b, double c, double d) {
    return a*x*x*x + b*x*x + c*x + d;
}

bool isDoubleZero(double num) {
    return fabs(num) < 1e-7;
}

void findX(double left, double right, double a, double b, double c, double d) {
    double valleft = jisuan(left, a, b, c, d);
    double valright = jisuan(right, a, b, c, d);
    if (isDoubleZero(valleft)) {
        res.push_back(left);
        return;
    }
    if (isDoubleZero(valright)) {
        res.push_back(right);
        return;
    }
    if (valleft * valright > 0) return;
    double mid;
    while (right - left > 0.001) {
        mid = (left + right) / 2.0;
        double valmid = jisuan(mid, a, b, c, d);
        if (isDoubleZero(valmid)) {
            res.push_back(mid);
            return;
        }
        if (valleft * valmid < 0)
            right = mid;
        else
            left = mid;
        valleft = jisuan(left, a, b, c, d);
        valright = jisuan(right, a, b, c, d);
    }
    res.push_back((left + right) / 2.0);
}

int main() {
    double a, b, c, d;
    cin >> a >> b >> c >> d;
    double left = -100, right;
    for (int i = 0; i < 300; ++i) {
        right = left + 1;
        double valleft = jisuan(left, a, b, c, d);
        double valright = jisuan(right, a, b, c, d);
        if (valleft * valright <= 0) {
            findX(left, right, a, b, c, d);
        }
        left = right;
    }
    sort(res.begin(), res.end());
    vector<double> roots;
    for (int i = 0; i < res.size(); ++i) {
        if (roots.empty() || fabs(res[i] - roots.back()) > 0.01) {
            roots.push_back(res[i]);
        }
    }
    for (int i = 0; i < 3; ++i) {
        printf("%.2f%c", roots[i], i==2?'\n':' ');
    }
    return 0;
}
