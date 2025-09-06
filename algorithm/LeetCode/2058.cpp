//
// Created by 86180 on 25-9-4.
//


#include <iostream>
#include <vector>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


vector<int> nodesBetweenCriticalPoints(ListNode* head) {
    vector<int> res = {-1, -1};
    vector<int> maxmin;
    ListNode *p=head->next;
    ListNode *pp=p->next;
    if(pp==nullptr||pp->next==nullptr){
        return res;
    }
    ListNode* back_p=pp->next;
    int front=p->val,mid=pp->val,back=back_p->val,address=2;
    while(back_p!=nullptr){
        back=back_p->val;
        if((mid<front&&mid<back)||(mid>front&&mid>back)){
            maxmin.push_back(address);
        }
        front=mid;
        mid=back;
        back_p=back_p->next;
    }
    int min=100000;
    if(maxmin.size()<2){
        return maxmin;
    }
    res[1]=maxmin[maxmin.size()-1]-maxmin[0];
    for(int i=0;i<maxmin.size();i++){
        int k=maxmin[i+1]-maxmin[i];
        if(k<min){
            min=k;
        }
    }
    res[0]=min;
    return res;
}




int main() {
    // 构造链表: 1 -> 3 -> 2 -> 2 -> 3 -> 2 -> 2 -> 2 -> 7
    ListNode* head = new ListNode(1);
    head->next = new ListNode(3);
    head->next->next = new ListNode(2);
    head->next->next->next = new ListNode(2);
    head->next->next->next->next = new ListNode(3);
    head->next->next->next->next->next = new ListNode(2);
    head->next->next->next->next->next->next = new ListNode(2);
    head->next->next->next->next->next->next->next = new ListNode(2);
    head->next->next->next->next->next->next->next->next = new ListNode(7);

    vector<int> res = nodesBetweenCriticalPoints(head);
    cout << "最小距离: " << res[0] << ", 最大距离: " << res[1] << endl;

    // 释放链表内存
    ListNode* p = head;
    while (p) {
        ListNode* tmp = p;
        p = p->next;
        delete tmp;
    }
    return 0;
}