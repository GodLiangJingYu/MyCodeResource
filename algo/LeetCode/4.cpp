//
// Created by 86180 on 25-7-9.
//
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

// 假设 findMedianSortedArrays 函数已实现
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    int sizeAll = nums1.size() + nums2.size();
    bool isOdd = false;
    if (sizeAll % 2 == 1) {
        isOdd = true;
    }
    int i = 0 , j = 0 ,sum = 0;
    double res = 0.0;
    if ( isOdd ) {
        //是奇数
        while ( sum != ( sizeAll+1 ) / 2 - 1 && i < nums1.size() && j < nums2.size() ) {
            if ( nums1[i] >= nums2[j]) {
                ++j;
                res = (double) nums1[i];
            }else {
                ++i;
                res = (double) nums2[j];
            }
            ++sum;
        }
    }
    else {
        //是偶数
        while ( sum != sizeAll / 2 - 1 && i < nums1.size() && j < nums2.size() ) {
            if ( nums1[i] >= nums2[j]) {
                ++j;
                res = (double) nums1[i];
            }else {
                ++i;
                res = (double) nums2[j];
            }
            ++sum;
        }
        res = ( nums1[i] + res) / 2.0;
    }
    return res;
}

vector<int> parseArray(const string& s) {
    vector<int> result;
    stringstream ss(s);
    int num;
    while (ss >> num) {
        result.push_back(num);
    }
    return result;
}

int main() {
    string line1, line2;
    getline(cin, line1);
    getline(cin, line2);

    vector<int> nums1 = parseArray(line1);
    vector<int> nums2 = parseArray(line2);

    double ans = findMedianSortedArrays(nums1, nums2);
    cout << ans << endl;

    return 0;
}