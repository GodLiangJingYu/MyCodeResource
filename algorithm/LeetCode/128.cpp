//
// Created by 86180 on 25-7-22.
//
#include<iostream>
#include <vector>

using namespace std;

int longestConsecutive(vector<int>& nums) {
    const int MAX_SIZE = 1e5;
    vector<bool> vectors(MAX_SIZE , false);
    for (auto num : nums) {
        if (!vectors[num]) {
            vectors[num] = true;
        }
    }
    int res=0, fin=0;
    for (auto vector : vectors) {
        if (vector) {
            ++fin;
        }else {
            if (res < fin) {
                res = fin;
            }
            fin = 0;
        }
    }
    return res;
}

int main() {
    vector<int> nums;
    int size;
    cout << "size:";
    cin >> size;
    cout <<"nums:"<<endl;
    for (int i = 0 ;i<size;i++) {
        int in;
        cin>> in;
        nums.push_back(in);
    }
    int res = longestConsecutive(nums);
    cout << res;
    return 0;
}