#include <iostream>
#include <stack>
using namespace std;

int main() {
    stack<int> stack;
    /*元素入栈*/
    stack.push(1);
    int top=stack.top();
    stack.pop();
    cout<< "栈顶元素" << top << endl;
}