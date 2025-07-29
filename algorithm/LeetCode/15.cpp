//
// Created by 86180 on 25-7-29.
//
#include<iostream>
#include<vector>
using namespace std;
vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> res;
    // 排序
    for (int i = 0; i < nums.size() - 1; i++) {
        for (int j = i; j < nums.size(); j++) {
            if (nums[j] > nums[j + 1]) {
                int reverse;
                reverse = nums[j + 1];
                nums[j + 1] = nums[j];
                nums[j] = reverse;
            }
        }
    }
    // 三指针
    int left = 0,right=nums.size(),mid=1;
    for (left;left<nums.size()-2;left++) {
        
    }
    return res;
}