//
// Created by 86180 on 25-9-15.
//
#include <bits/stdc++.h>
using namespace std;
int solution(vector<vector<int>>& circles) {
    set<pair<int, int>> covered;
    for (const auto& c : circles) {
        int cx = c[0], cy = c[1], r = c[2];
        for (int x = cx - r; x <= cx + r; ++x) {
            for (int y = cy - r; y <= cy + r; ++y) {
                if ((x - cx)*(x - cx) + (y - cy)*(y - cy) <= r*r) {
                    covered.insert({x, y});
                }
            }
        }
    }
    return covered.size();
}

int main()
{
    int n;
    cin >> n;
    vector<vector<int>> arr(n, vector<int>(3));
    for(int i=0;i<arr.size();i++){
        cin>>arr[i][0]>>arr[i][1]>>arr[i][2];
    }

    cout << solution(arr) << endl;
    return 0;
}