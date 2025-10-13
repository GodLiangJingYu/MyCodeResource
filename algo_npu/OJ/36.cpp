//
// Created by 86180 on 25-10-11.
//
#include <iostream>
#include <vector>
#include <algorithm>  // for std::max

using namespace std;

int main() {
    int T, N;
    cin >> T >> N;

    vector<pair<int, int>> items(N);  // pair<time, value>
    for (int i = 0; i < N; ++i) {
        cin >> items[i].first >> items[i].second;
    }

    // DP: dp[i][j] = max value using first i items, time <= j
    vector<vector<int>> dp(N + 1, vector<int>(T + 1, 0));

    for (int i = 1; i <= N; ++i) {
        int time = items[i - 1].first;
        int value = items[i - 1].second;
        for (int j = 0; j <= T; ++j) {
            // 不选
            dp[i][j] = dp[i - 1][j];
            // 选
            if (j >= time) {
                dp[i][j] = max(dp[i][j], dp[i - 1][j - time] + value);
            }
        }
    }

    cout << dp[N][T] << endl;
    return 0;
}