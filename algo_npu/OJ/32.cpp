//
// Created by 86180 on 25-9-25.
//
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

// 定义单链表节点结构
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// 将数组转换为链表
ListNode* arrayToLinkedList(vector<int>& arr) {
    if (arr.empty()) {
        return nullptr;
    }
    ListNode* dummyHead = new ListNode(0);
    ListNode* current = dummyHead;
    for (int value : arr) {
        current->next = new ListNode(value);
        current = current->next;
    }
    ListNode* head = dummyHead->next;
    delete dummyHead; // 释放哑节点
    return head;
}

// 将链表转换为数组
vector<int> linkedListToArray(ListNode* head) {
    vector<int> result;
    ListNode* current = head;
    while (current != nullptr) {
        result.push_back(current->val);
        current = current->next;
    }
    return result;
}

// 两个链表表示的数字相加
ListNode* nodeSummation(ListNode* l1, ListNode* l2, int carry = 0) {
    // 递归终止条件：两个链表都为空且没有进位
    if (l1 == nullptr && l2 == nullptr && carry == 0) {
        return nullptr;
    }

    // 获取当前节点的值，空节点视为 0
    int val1 = (l1 != nullptr) ? l1->val : 0;
    int val2 = (l2 != nullptr) ? l2->val : 0;

    // 计算当前位的和和新的进位
    int sum = val1 + val2 + carry;
    carry = sum / 10;
    int digit = sum % 10;

    // 创建当前节点
    ListNode* node = new ListNode(digit);

    // 递归处理下一位
    node->next = nodeSummation(
        (l1 != nullptr) ? l1->next : nullptr,
        (l2 != nullptr) ? l2->next : nullptr,
        carry
    );

    return node;
}

int main() {
    string line;
    vector<int> arr1, arr2;

    // 读取第一行输入 (l1)
    getline(cin, line);
    stringstream ss1(line);
    int num;
    while (ss1 >> num) {
        arr1.push_back(num);
    }

    // 读取第二行输入 (l2)
    getline(cin, line);
    stringstream ss2(line);
    while (ss2 >> num) {
        arr2.push_back(num);
    }

    // 将数组转换为链表
    ListNode* l1 = arrayToLinkedList(arr1);
    ListNode* l2 = arrayToLinkedList(arr2);

    // 调用节点加和函数
    ListNode* result = nodeSummation(l1, l2);

    // 将结果链表转换为数组以便输出
    vector<int> resultArr = linkedListToArray(result);

    // 输出结果
    for (size_t i = 0; i < resultArr.size(); ++i) {
        if (i > 0) cout << " ";
        cout << resultArr[i];
    }
    cout << endl;

    // 清理内存，释放链表
    while (l1 != nullptr) {
        ListNode* temp = l1;
        l1 = l1->next;
        delete temp;
    }
    while (l2 != nullptr) {
        ListNode* temp = l2;
        l2 = l2->next;
        delete temp;
    }
    while (result != nullptr) {
        ListNode* temp = result;
        result = result->next;
        delete temp;
    }

    return 0;
}