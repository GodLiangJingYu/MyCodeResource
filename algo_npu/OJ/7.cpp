//
// Created by 86180 on 25-9-15.
//
#include <bits/stdc++.h>
using namespace std;

int solution(vector<int>& nums) {
    int total=0;
    for (auto num:nums) {
        total+=num;
    }
    unordered_set<int> setnum(nums.begin(),nums.end());
    int maxval=INT_MIN;
    for (auto num:nums) {
        int val=total-2*num;
        if (setnum.count(val)&&val!=num) {
            maxval=max(maxval, val);
        }
    }
    if (maxval==INT_MIN) {
        maxval = *max_element(nums.begin(), nums.end());
    }
    return maxval;
}



int main()
{
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++){
        cin >> arr[i];
    }
    cout << solution(arr) << endl;
    return 0;
}