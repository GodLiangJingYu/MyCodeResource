//
// Created by 86180 on 25-9-25.
//
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 读取链表输入，如 "3->6->4->1"
ListNode* readList(const string& input) {
    if (input.empty())
        return nullptr;
    stringstream ss(input);
    vector<int> vals;
    string token;
    while (getline(ss, token, '-')) {
        if (!token.empty() && token != ">") {
            if (token[0] == '>') token = token.substr(1);
            vals.push_back(stoi(token));
        }
    }
    if (vals.empty())
        return nullptr;
    ListNode* head = new ListNode(vals[0]);
    ListNode* curr = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        curr->next = new ListNode(vals[i]);
        curr = curr->next;
    }
    return head;
}

// 空白子函数（你可以在这里实现功能，比如反转链表）
vector<int> solve(ListNode* head) {
    // TODO: 实现你的功能

}

int main() {
    string input;
    getline(cin, input); // 读取链表输入
    ListNode* head = readList(input);

    vector<int> result = solve(head);

    // 输出结果
    if (result.empty()) {
        cout << endl;
    } else {
        for (size_t i = 0; i < result.size(); ++i) {
            if (i) cout << " ";
            cout << result[i];
        }
        cout << endl;
    }

    // 释放链表内存
    ListNode* curr = head;
    while (curr) {
        ListNode* tmp = curr;
        curr = curr->next;
        delete tmp;
    }
    return 0;
}