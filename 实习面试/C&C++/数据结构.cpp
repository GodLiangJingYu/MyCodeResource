//
// Created by 86180 on 25-6-20.
//
#include <iostream>
using namespace std;

int main() {
    /*数组*/
    //储存在栈上
    int arr[5];//arr中都是0
    int nums[5]={1,2,3,4,5};
    //储存在堆上,手动释放空间
    int* arr1=new int[5];
    int* nums1=new int[5] {1,2,3,4,5};
    return 0;
}