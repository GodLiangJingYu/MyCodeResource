//
// Created by 86180 on 25-9-19.
//
#include <iostream>
#include <stack>
using namespace std;

stack<int> stks;

void addStk() {
    int input;
    cin>>input;
    if (input==0) {
        return;
    }
    stks.push(input);
    return addStk();
}

int main() {
    addStk();
    while (stks.size()>1) {
        cout<<stks.top()<<' ';
        stks.pop();
    }
    cout<<stks.top();
    return 0;
}