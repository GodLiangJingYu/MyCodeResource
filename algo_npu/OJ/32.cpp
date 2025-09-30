//
// Created by 86180 on 25-9-25.
//
#include<iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
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



int main() {
    string inputA;
    getline(cin, inputA); // 读取链表输入
    ListNode* headA = readList(inputA);
    string inputB;
    getline(cin, inputB); // 读取链表输入
    ListNode* headB = readList(inputB);




    // 释放链表内存
    ListNode* curra = headA;
    while (curra) {
        ListNode* tmp = curra;
        curra = curra->next;
        delete tmp;
    }
    ListNode* currb = headB;
    while (currb) {
        ListNode* tmp = currb;
        currb = currb->next;
        delete tmp;
    }
    return 0;
}