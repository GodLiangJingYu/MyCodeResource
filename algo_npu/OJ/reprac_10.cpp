//
// Created by 86180 on 25-9-16.
//
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (ll)4e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    long long numOps;
    if (!(cin >> s)) return 0;
    cin >> numOps;
    int n = (int)s.size();

    // 前缀 1 的个数
    vector<int> pref1(n+1, 0);
    for (int i = 1; i <= n; ++i) pref1[i] = pref1[i-1] + (s[i-1] == '1');

    // 给定 K，返回将 s 变为最长段长度 <= K 的最少翻转数
    auto minOpsForK = [&](int K)->long long {
        vector<ll> dp0(n+1, INF), dp1(n+1, INF);
        // dp0[i] = 最少翻转次数使前缀长度为 i，且以 '0' 结尾（最后一段长度 <= K）
        // dp1[i] = 最少翻转次数使前缀长度为 i，且以 '1' 结尾
        dp0[0] = dp1[0] = 0;

        deque<pair<ll,int>> dq0, dq1; // (value, index)
        for (int i = 1; i <= n; ++i) {
            int j = i - 1;
            // 把 j 加入队列（作为窗口右端新元素）
            ll v0 = dp1[j] - pref1[j];
            while (!dq0.empty() && dq0.back().first >= v0) dq0.pop_back();
            dq0.emplace_back(v0, j);

            ll v1 = dp0[j] + pref1[j] - j;
            while (!dq1.empty() && dq1.back().first >= v1) dq1.pop_back();
            dq1.emplace_back(v1, j);

            // 窗口左端为 i-K，弹出过期索引
            int min_index = i - K;
            while (!dq0.empty() && dq0.front().second < min_index) dq0.pop_front();
            while (!dq1.empty() && dq1.front().second < min_index) dq1.pop_front();

            if (!dq0.empty()) dp0[i] = pref1[i] + dq0.front().first;
            if (!dq1.empty()) dp1[i] = (i - pref1[i]) + dq1.front().first;
        }

        return min(dp0[n], dp1[n]);
    };

    int left = 1, right = n, ans = n;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        long long need = minOpsForK(mid);
        if (need <= numOps) {
            ans = mid;
            right = mid - 1;
        } else left = mid + 1;
    }

    cout << ans << '\n';
    return 0;
}
