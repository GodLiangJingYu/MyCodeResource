//
// Created by 86180 on 25-9-25.
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// 定义链表节点结构
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// 将输入字符串转换为链表
ListNode* stringToList(string s) {
    // 去除首尾方括号（如果存在）
    if (s.length() >= 2 && s[0] == '[' && s[s.length() - 1] == ']') {
        s = s.substr(1, s.length() - 2);
    }
    if (s.empty()) return nullptr;

    // 使用 stringstream 解析数字
    stringstream ss(s);
    int num;
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (ss >> num) {
        tail->next = new ListNode(num);
        tail = tail->next;
    }
    // 检查是否有未解析的非法字符
    string remaining;
    ss >> remaining;
    if (!remaining.empty()) {
        // 清理已分配的节点并返回空链表
        while (dummy.next) {
            ListNode* temp = dummy.next;
            dummy.next = temp->next;
            delete temp;
        }
        return nullptr;
    }
    return dummy.next;
}

// 反转链表并返回所有节点值的数组
vector<int> reverseLinkedList(ListNode* head) {
    vector<int> result;

    // 特殊情况：空链表
    if (!head) {
        return result;
    }

    // 反转链表
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while (curr) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    // 遍历反转后的链表，收集所有节点值
    ListNode* newHead = prev;
    while (newHead) {
        result.push_back(newHead->val);
        newHead = newHead->next;
    }

    return result;
}

int main() {
    string input;
    getline(cin, input); // 读取整行输入
    ListNode* list = stringToList(input);
    vector<int> result = reverseLinkedList(list);

    // 输出结果，数字间用空格分隔
    if (result.empty()) {
        // 对于空链表，输出空行（根据样例 3）
        cout << endl;
    } else {
        for (size_t i = 0; i < result.size(); ++i) {
            if (i > 0) cout << " ";
            cout << result[i];
        }
        cout << endl;
    }

    // 释放链表内存
    ListNode* curr = list;
    while (curr) {
        ListNode* temp = curr;
        curr = curr->next;
        delete temp;
    }

    return 0;
}