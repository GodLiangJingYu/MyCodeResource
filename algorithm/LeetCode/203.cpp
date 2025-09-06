//
// Created by 86180 on 25-9-5.
//
#include <iostream>
#include <vector>

using namespace std;

// ListNode 结构体定义
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// removeElements 函数原型声明
ListNode* removeElements(ListNode* head, int val) {
    if (head == nullptr) {
        return head;
    }
    while (head->val==val) {
        head=head->next;
    }
    ListNode *pf=head;
    ListNode *pb=pf->next;
    while (pb!=nullptr) {
        if (pb->val==val) {
            pf->next=pb->next;
            pb=pb->next;
            continue;
        }
        pf=pb;
        pb=pb->next;
    }
    return head;
}

// 辅助函数：打印链表
void printList(ListNode* head) {
    while (head != nullptr) {
        cout << head->val << " ";
        head = head->next;
    }
    cout << endl;
}

// 主函数用于测试
int main() {
    // 创建链表 1->2->6->3->4->5->6
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(6);
    head->next->next->next = new ListNode(3);
    head->next->next->next->next = new ListNode(4);
    head->next->next->next->next->next = new ListNode(5);
    head->next->next->next->next->next->next = new ListNode(6);

    // 测试用例 1: 移除值为 6 的节点
    int val1 = 6;
    cout << "Test Case 1: Expected [1 2 3 4 5], Got ";
    head = removeElements(head, val1);
    printList(head);

    // 重置链表指针以复用相同的链表结构
    ListNode* temp = head;
    while (temp != nullptr) {
        temp->val = 0; // 重置节点值
        temp = temp->next;
    }

    // 更新链表值为空链表
    head->val = 0;
    head->next = nullptr;

    // 测试用例 2: 空链表
    int val2 = 1;
    cout << "Test Case 2: Expected [], Got ";
    head = removeElements(head, val2);
    printList(head);

    // 重置链表指针以复用相同的链表结构
    temp = head;
    while (temp != nullptr) {
        temp->val = 0; // 重置节点值
        temp = temp->next;
    }

    // 更新链表值为 7->7->7->7
    head->val = 7;
    head->next = new ListNode(7);
    head->next->next = new ListNode(7);
    head->next->next->next = new ListNode(7);

    // 测试用例 3: 移除所有值为 7 的节点
    int val3 = 7;
    cout << "Test Case 3: Expected [], Got ";
    head = removeElements(head, val3);
    printList(head);

    // 清理内存
    while (head != nullptr) {
        ListNode* toDelete = head;
        head = head->next;
        delete toDelete;
    }

    return 0;
}