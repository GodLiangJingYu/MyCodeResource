//
// Created by 86180 on 25-9-14.
//
#include <algorithm>
#include <iostream>
using namespace std;

int solution(int n, int a[]) {
    int res = 0;
    if (n<3) {
        return 0;
    }
    sort(a, a + n);
    for (int i=n-1;i>1;i--) {
        int right = i-1 , left = 0;
        while (left < right) {
            if (a[left]+a[right]>a[i]) {
                --right;
            }else if (a[left]+a[right]<a[i]) {
                ++left;
            }else {
                res++;
                break;
            }
        }
    }
    return res;
}



int main()
{
    int n;
    int a[505];
    cin >> n;
    for (int i = 0; i < n; i++){
        cin >> a[i];
    }
    cout << solution(n, a) << endl;
    return 0;
}