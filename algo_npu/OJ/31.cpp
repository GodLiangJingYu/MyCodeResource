//
// Created by 86180 on 25-9-25.
//
/*
 * 0 1 6 8 9
 * 11 88 69 96
 * 101 111 181 808 818 888 609 619 689 926 916 986
 * 1001 1111 1881 1691
 */
#include <iostream>
#include <vector>
using namespace std;
vector<int> res;
int inNum;

void findReverse(int minNum) {
    int maxNum=10*minNum;

}

int main() {
    cin>>inNum;
    int input=inNum;
    int minNum=1;
    while (input>1) {
        minNum*=10;
        input--;
    }
    findReverse(minNum);
    for (int i=0;i<res.size();i++) {
        cout<<res[i];
        if (i!=res.size()-1) {
            cout<<' ';
        }
    }
    return 0;
}