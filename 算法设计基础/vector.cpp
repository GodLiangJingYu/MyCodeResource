//
// Created by 86180 on 25-6-30.
//
#include <algorithm>
#include <deque>
#include<iostream>
#include <vector>
using namespace std;

int main() {
    deque<int> deque;

    /* 元素入队 */
    deque.push_back(2);   // 添加至队尾
    deque.push_back(5);
    deque.push_back(4);
    deque.push_front(3);  // 添加至队首
    deque.push_front(1);

    /* 访问元素 */
    int front = deque.front(); // 队首元素
    int back = deque.back();   // 队尾元素
    cout<<front<<" "<<back<<endl;
    return 0;
}