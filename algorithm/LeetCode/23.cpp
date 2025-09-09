//
// Created by 86180 on 25-9-7.
//
#include <iostream>
#include <vector>
#include <queue>
#include <functional>
using namespace std;
// 定义链表节点结构
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 比较器，用于优先队列
struct CompareNode {
    bool operator()(const ListNode* a, const ListNode* b) {
        return a->val > b->val;
    }
};

// 合并 K 个升序链表的函数（待实现）
ListNode* mergeKLists(vector<ListNode*>& lists) {
    
}

// 打印链表
void printList(ListNode* head) {
    while (head) {
        cout << head->val << " ";
        head = head->next;
    }
    cout << endl;
}

int main() {
    // 创建示例链表
    ListNode* list1 = new ListNode(1);
    list1->next = new ListNode(4);
    list1->next->next = new ListNode(5);

    ListNode* list2 = new ListNode(1);
    list2->next = new ListNode(3);
    list2->next->next = new ListNode(4);

    ListNode* list3 = new ListNode(2);
    list3->next = new ListNode(6);

    // 将链表放入向量
    vector<ListNode*> lists = {list1, list2, list3};

    // 调用 mergeKLists 函数
    ListNode* mergedList = mergeKLists(lists);

    // 打印合并后的链表
    printList(mergedList);

    // 释放内存
    while (mergedList) {
        ListNode* temp = mergedList;
        mergedList = mergedList->next;
        delete temp;
    }

    return 0;
}