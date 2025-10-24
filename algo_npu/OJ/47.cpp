//
// Created by 86180 on 25-10-20.
//
#include <iostream>
#include <vector>
#include <sstream>
#include <climits>
#include <algorithm>

using namespace std;

long long maxBattlePower(vector<int>& abilities) {
    int n = abilities.size();
    if (n == 0) return 0;
    vector<vector<long long>> dp(2, vector<long long>(n, 0));
    dp[0][0] = dp[1][0] = abilities[0];
    for (int i = 1; i < n; ++i) {
        long long curr = abilities[i];
        long long max_with_curr = curr;
        long long min_with_curr = curr;
        if (dp[0][i-1] != LLONG_MIN) {
            max_with_curr = max(max_with_curr, max(dp[0][i-1] * curr, dp[1][i-1] * curr));
            min_with_curr = min(min_with_curr, min(dp[0][i-1] * curr, dp[1][i-1] * curr));
        }
        dp[0][i] = dp[0][i-1] != LLONG_MIN ? max(max_with_curr, dp[0][i-1]) : max_with_curr;
        dp[1][i] = dp[1][i-1] != LLONG_MIN ? min(min_with_curr, dp[1][i-1]) : min_with_curr;
    }

    return dp[0][n-1];
}

int main() {
    string input;
    getline(cin, input);
    stringstream ss(input);
    vector<int> abilities;
    int num;
    while (ss >> num) {
        abilities.push_back(num);
    }

    cout << maxBattlePower(abilities) << endl;
    return 0;
}
