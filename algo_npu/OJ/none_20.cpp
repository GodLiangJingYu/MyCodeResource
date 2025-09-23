//
// Created by 86180 on 25-9-19.
//
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> canSees;
    vector<int> buildings(n, 0);
    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        buildings[x - 1] = y;
        int canSee = 0;
        double maxSlope = -1.0;
        for (int j = 0; j < n; ++j) {
            if (buildings[j] > 0) {
                double slope = (double)buildings[j] / (j + 1);
                if (slope > maxSlope) {
                    canSee++;
                    maxSlope = slope;
                }
            }
        }
        canSees.push_back(canSee);
    }
    for (auto canSe:canSees) {
        cout<<canSe<<endl;
    }
    return 0;
}
