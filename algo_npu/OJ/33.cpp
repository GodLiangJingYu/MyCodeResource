//
// Created by 86180 on 25-10-11.
//
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

// 检查数组是否为二叉搜索树的先序遍历序列，使用递归方法，需由你实现
bool checkPreorderOfBST(vector<int>& arr, int start, int end) {
    if (start > end) return true;
    if (start == end) return true;
    int root = arr[start];
    int i = start + 1;
    while (i <= end && arr[i] < root) {
        i++;
    }
    for (int j = i; j <= end; j++) {
        if (arr[j] <= root) return false;
    }
    return checkPreorderOfBST(arr, start + 1, i - 1) && checkPreorderOfBST(arr, i, end);
}

// 包装函数，简化调用
bool checkPreorderOfBST(vector<int>& arr) {
    if (arr.empty()) return true; // 空数组视为有效
    return checkPreorderOfBST(arr, 0, arr.size() - 1);
}

int main() {
    string line;
    vector<int> arr;

    // 读取一行输入
    getline(cin, line);
    stringstream ss(line);
    int num;
    while (ss >> num) {
        arr.push_back(num);
    }

    // 调用检查函数
    bool result = checkPreorderOfBST(arr);

    // 输出结果
    cout << (result ? "true" : "false") << endl;

    return 0;
}