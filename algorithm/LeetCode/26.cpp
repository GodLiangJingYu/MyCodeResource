//
// Created by 86180 on 25-9-9.
//
#include <iostream>
#include <vector>
#include <assert.h>
#include <unordered_map>

// 你需要补全的 removeDuplicates 函数
int removeDuplicates(std::vector<int>& nums) {
    std::unordered_map<int,bool> hash_map;
    int deefence=0,insert=0;
    for (auto num:nums) {
        if (hash_map[num]==false) {
            hash_map[num]=true;
            nums[insert++]=num;
        }
        deefence++;
    }
    return insert;
}

// 辅助函数：打印数组
void printVector(const std::vector<int>& vec) {
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

// main 函数
int main() {
    // 示例 1
    std::vector<int> nums1 = {1, 1, 2};
    int k1 = removeDuplicates(nums1);
    std::cout << "示例 1 结果: " << k1 << ", nums = [";
    printVector(nums1);
    assert(k1 == 2 && nums1[0] == 1 && nums1[1] == 2);

    // 示例 2
    std::vector<int> nums2 = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    int k2 = removeDuplicates(nums2);
    std::cout << "示例 2 结果: " << k2 << ", nums = [";
    printVector(nums2);
    assert(k2 == 5 && nums2[0] == 0 && nums2[1] == 1 && nums2[2] == 2 && nums2[3] == 3 && nums2[4] == 4);

    // 示例 3
    std::vector<int> nums3 = {1, 1, 1, 1, 1};
    int k3 = removeDuplicates(nums3);
    std::cout << "示例 3 结果: " << k3 << ", nums = [";
    printVector(nums3);
    assert(k3 == 1 && nums3[0] == 1);

    // 示例 4
    std::vector<int> nums4 = {};
    int k4 = removeDuplicates(nums4);
    std::cout << "示例 4 结果: " << k4 << ", nums = [";
    printVector(nums4);
    assert(k4 == 0);

    std::cout << "所有测试用例通过！" << std::endl;

    return 0;
}
