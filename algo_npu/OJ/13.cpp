//
// Created by 86180 on 25-9-16.
//
#include <iostream>
#include <vector>
using namespace std;

void rev(vector<int> &vec,int k) {
    if (k==1) {
        return;
    }else {
        swap(vec[0],vec[1]);
        vec[1]+=vec[0];
        rev(vec,--k);
        return;
    }
}

int main() {
    int k;
    cin>>k;
    vector<int> vector{1,1};
    rev(vector,k);
    cout<<vector[0]<<' '<<vector[1];
    return 0;
}