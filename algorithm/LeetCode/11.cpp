//
// Created by 86180 on 25-7-9.
//
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

int maxArea(vector<int>& height) {
    return 0;
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