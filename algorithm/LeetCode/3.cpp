//
// Created by 86180 on 25-7-15.
//
#include <iostream>
#include <string>
#include <unordered_map>
#define MAX 50000;
using namespace std;

int lengthOfLongestSubstring(string s) {
    unordered_map<char, int> charIndexMap;
    int maxLength = 0, start = 0;
    for (int i = 0; i < s.length(); i++) {
        if (charIndexMap.count(s[i]) && charIndexMap[s[i]] >= start) {
            start = charIndexMap[s[i]] + 1;
        }
        charIndexMap[s[i]] = i;
        maxLength = max(maxLength, i - start + 1);
    }
    return maxLength;
}

int main() {
    string s;
    cout << "cin: ";
    cin >> s;
    cout << lengthOfLongestSubstring(s) << endl;
    return 0;
}