//
// Created by 86180 on 25-7-9.
//
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

int maxArea(vector<int>& height) {
    int left = 0, right = height.size() - 1, res = 0;
    while (left < right) {
        int h = min(height[left], height[right]);
        res = max(res, h * (right - left));
        if (height[left] < height[right])
            ++left;
        else
            --right;
    }
    return res;
}

vector<int> parseInput(const string& line) {
    vector<int> res;
    stringstream ss(line);
    int x;
    while (ss >> x) res.push_back(x);
    return res;
}

int main() {
    string line;
    getline(cin, line);
    vector<int> height = parseInput(line);

    int ans = maxArea(height);
    cout << ans << endl;
    return 0;
}