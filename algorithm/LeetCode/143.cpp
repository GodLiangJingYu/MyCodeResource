//
// Created by 86180 on 25-9-5.
//
#include <iostream>
using namespace std;

// 定义链表节点
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 用于打印链表
void printList(ListNode* head) {
    while (head) {
        cout << head->val << " ";
        head = head->next;
    }
    cout << endl;
}

// 重排链表函数
void reorderList(ListNode* head) {
//    方法一：
     if (!head||!head->next||!head->next->next) {
        return;
    }
    ListNode *p=head;
    ListNode *pp=p->next;
    while (pp->next) {
        p=pp;
        pp=pp->next;
    }
    pp->next=head->next;
    head->next=pp;
    p->next=nullptr;
    reorderList(pp->next);
    /*
    if (!head || !head->next) return;
    ListNode *slow = head, *fast = head, *prev = nullptr;
    while (fast && fast->next) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    if (prev) prev->next = nullptr;
    ListNode *cur = slow, *nxt = nullptr, *newHead = nullptr;
    while (cur) {
        nxt = cur->next;
        cur->next = newHead;
        newHead = cur;
        cur = nxt;
    }
    ListNode *first = head, *second = newHead;
    while (first && second) {
        ListNode *tmp1 = first->next;
        ListNode *tmp2 = second->next;
        first->next = second;
        if (!tmp1) break;
        second->next = tmp1;
        first = tmp1;
        second = tmp2;
    }*/
}

int main() {
    // 创建链表 1 -> 2 -> 3 -> 4
    ListNode *head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);
    head->next->next->next = new ListNode(4);

    cout << "Original list: ";
    printList(head);

    reorderList(head);

    cout << "Reordered list: ";
    printList(head);

    return 0;
}