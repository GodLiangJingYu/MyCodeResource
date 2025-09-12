#include <iostream>
using namespace std;

// 定义链表节点结构
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// 旋转链表函数
ListNode* rotateRight(ListNode* head, int k) {

}

// 辅助函数：打印链表
void printList(ListNode* head) {
    while (head) {
        cout << head->val << " ";
        head = head->next;
    }
    cout << endl;
}

// 主函数
int main() {
    // 测试用例1
    ListNode* head1 = new ListNode(1);
    head1->next = new ListNode(2);
    head1->next->next = new ListNode(3);
    head1->next->next->next = new ListNode(4);
    head1->next->next->next->next = new ListNode(5);
    int k1 = 2;
    cout << "测试用例1的结果: ";
    printList(rotateRight(head1, k1)); // 预期输出: 4 5 1 2 3

    // 测试用例2
    ListNode* head2 = new ListNode(0);
    head2->next = new ListNode(1);
    head2->next->next = new ListNode(2);
    int k2 = 4;
    cout << "测试用例2的结果: ";
    printList(rotateRight(head2, k2)); // 预期输出: 2 0 1

    return 0;
}//
// Created by 86180 on 25-9-11.
//
