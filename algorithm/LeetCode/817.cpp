//
// Created by 86180 on 25-9-5.
//
#include <iostream>
#include <unordered_map>
#include <vector>
#include <unordered_set>

using namespace std;

// ListNode 结构体定义
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// numComponents 函数原型声明
int numComponents(ListNode* head, vector<int>& nums){
    unordered_map<int,bool> hash_map;
    for (int i=0;i<nums.size();i++) {
        hash_map[nums[i]]=true;
    }
    if(head->next==nullptr){
        if(hash_map[head->val]==true){
            return 1;
        }else{
            return 0;
        }
    }
    ListNode* p=head;
    int res=0;
    bool count=false;
    while (p!=nullptr) {
        if (hash_map[p->val]==true) {
            if (count==false) {
                count=true;
                res++;
            }
        }else {
            if (count==true) {
                count=false;
            }
        }
        p=p->next;
    }
    return res;
}

// 主函数用于测试
int main() {
    // 创建链表 0->1->2->3
    ListNode* head = new ListNode(0);
    head->next = new ListNode(1);
    head->next->next = new ListNode(2);
    head->next->next->next = new ListNode(3);

    // 测试用例 1
    vector<int> nums1 = {0, 2};
    cout << "Test Case 1: Expected 2, Got " << numComponents(head, nums1) << endl;

    // 重置链表指针以复用相同的链表结构
    ListNode* temp = head;
    while (temp != nullptr) {
        temp->val = 0; // 重置节点值
        temp = temp->next;
    }

    // 更新链表值为 0->1->2->3->4
    head->val = 0;
    head->next->val = 1;
    head->next->next->val = 2;
    head->next->next->next->val = 3;
    head->next->next->next->next = new ListNode(4);

    // 测试用例 2
    vector<int> nums2 = {0, 3, 1, 4};
    cout << "Test Case 2: Expected 2, Got " << numComponents(head, nums2) << endl;

    // 清理内存
    while (head != nullptr) {
        ListNode* toDelete = head;
        head = head->next;
        delete toDelete;
    }

    return 0;
}