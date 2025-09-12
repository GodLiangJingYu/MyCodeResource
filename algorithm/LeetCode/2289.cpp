//
// Created by 86180 on 25-9-11.
//
#include <iostream>
#include <vector>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

int finddepth(ListNode* head, int depth) {
    ListNode* p = head;
    bool res = false;
    vector<int> vec={0,0};
    while (p->next != nullptr && p->next->next != nullptr) {
        if (p->next->val < p->val||(vec[0]==1&&vec[1]>p->next->val)) {
            ListNode* removeNode = p->next;
            p->next = removeNode->next;
            vec[0]=1;
            vec[1]=removeNode->val;
            delete removeNode;
            res = true;
        }else if (vec[0]==1&&vec[1]<=p->next->val) {
            vec[0]=0;
            p=p->next;
        }else{
            p = p->next;
        }
    }
    if (res) {
        return finddepth(head, depth + 1);
    }
    return depth;
}

int totalSteps(vector<int>& nums) {
    if (nums.empty())
        return 0;
    ListNode* head = new ListNode();
    ListNode* p = head;
    for (auto num : nums) {
        p->next = new ListNode(num);
        p = p->next;
    }
    int res, depth = 0;
    res = finddepth(head->next, depth);
    while (head != nullptr) {
        ListNode* relese = head;
        head = head->next;
        delete relese;
    }
    return res;
}

int main() {
    // 测试用例1
    vector<int> nums1 = {5,3,4,4,7,3,6,11,8,5,11};
    cout << "测试用例1的结果: " << totalSteps(nums1) << endl; // 预期输出: 3

    // 测试用例2
    vector<int> nums2 = {10,6,5,10,15};
    cout << "测试用例2的结果: " << totalSteps(nums2) << endl; // 预期输出: 0

    return 0;
}