//
// Created by 86180 on 25-9-19.
//
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <climits>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, M;
    cin >> N >> M;

    vector<long long> heights(N + 1, 0); // 楼房编号从1到N

    for (int day = 0; day < M; ++day) {
        int x, y;
        cin >> x >> y;
        heights[x] = y;

        // 重新计算可见楼房数量
        int visible_count = 0;
        double last_slope = -1.0; // 初始化为一个极小值

        for (int i = 1; i <= N; ++i) {
            if (heights[i] > 0) { // 只考虑高度大于0的楼房
                double current_slope = (double)heights[i] / i;
                if (current_slope > last_slope) {
                    visible_count++;
                    last_slope = current_slope;
                }
            }
        }

        cout << visible_count << "\n";
    }

    return 0;
}