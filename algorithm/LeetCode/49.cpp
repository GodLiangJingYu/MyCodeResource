//
// Created by 86180 on 25-7-20.
//
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

vector<vector<string>> groupAnagrams(vector<string>& strs) {
    vector<unordered_map<char , int>> hashs;
    vector<vector<string>> res;
    vector<unordered_map<char, int>> selects;
    if (strs.size() == 0) {
        res[0][0] = nullptr;
        return res;
    }
    if (strs.size() == 1) {
        res[0].push_back(strs[0]);
        return res;
    }
    for (auto str : strs) {
        unordered_map<char , int> getIn;
        for (int i = 0 ; i < str.size() ; i++) {
            getIn[str[i]] = 1;
        }
        hashs.push_back(getIn);
    }
    int i = 0;
    for (int num = 0 ; num < hashs.size() ; num++) {
        for (int j = 0 ; j < selects.size() ; j++) {
            if (hashs[num] == selects[j]) {
                res[j].push_back(strs[num]);
                break;
            }
            if (j == selects.size() - 1) {
                selects.push_back(hashs[num]);
                res[i++].push_back(strs[num]);
            }
        }
    }
    return res;
}

int main() {
    vector<string> strs;
    int n;
    cout << "请输入字符串个数: ";
    cin >> n;
    cout << "请输入每个字符串:" << endl;
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        strs.push_back(s);
    }
    vector<vector<string>> result = groupAnagrams(strs);
    cout << "分组结果:" << endl;
    for (const auto& group : result) {
        cout << "[";
        for (size_t i = 0; i < group.size(); ++i) {
            cout << group[i];
            if (i != group.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
    return 0;
}