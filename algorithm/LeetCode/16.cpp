//
// Created by 86180 on 25-9-5.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;


int threeSumClosest(vector<int>& nums, int target) {
    sort(nums.begin(),nums.end());
    int sum=0;
    for(int i=0,j=0;i<nums.size()&&j<3;++i){
        if(i==nums.size()-1&&nums[i]<target){
            target=target-nums[i];
            sum=sum+nums[i];
            j++;
            nums.erase(nums.begin()+i);
            i=i-1;
        }
        else if(i==0&&target<nums[i]){
            sum=nums[i]+nums[i+1]+nums[i+2];
            break;
        }
        else if(i<nums.size()-1&&i>0&&target>nums[i]&&target<nums[i+1]){
            target=target-nums[i]-nums[i+1];
            j=j+2;
            sum=sum+nums[i]+nums[i+1];
            nums.erase(nums.begin()+i);
            nums.erase(nums.begin()+i);
            i=0;
        }
        else if(i<nums.size()-1&&i>0&&target==nums[i]){
            target=target-nums[i];
            j++;
            sum=sum+nums[i];
            nums.erase(nums.begin()+i);
            i=i-1;
        }
    }
    return sum;
}


int main() {
    vector<int> nums1 = {-1, 2, 1, -4};
    int target1 = 1;
    cout << "Example 1: " << threeSumClosest(nums1, target1) << endl; // 输出应该是 2

    vector<int> nums2 = {0, 0, 0};
    int target2 = 1;
    cout << "Example 2: " << threeSumClosest(nums2, target2) << endl; // 输出应该是 0

    return 0;
}
