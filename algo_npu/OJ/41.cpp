//
// Created by 86180 on 25-9-22.
//
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

// 剔除法实现
int removeMethodMaxProfit(const vector<int>& arr) {
    int left = 0, right = arr.size() - 1;
    int currSum = 0;
    for (int num : arr) currSum += num; // 总和
    int maxSum = currSum;

    while (left < right) {
        // 比较左右两端哪个“更不利”
        if (arr[left] < arr[right]) {
            currSum -= arr[left];
            left++;
        } else {
            currSum -= arr[right];
            right--;
        }
        if (currSum > maxSum) maxSum = currSum;
    }
    return maxSum;
}

int main() {
    vector<int> arr;
    string line;
    getline(cin, line);             // 读入一整行
    istringstream iss(line);
    int x;
    while (iss >> x) arr.push_back(x);
    cout << removeMethodMaxProfit(arr) << endl;
    return 0;
}