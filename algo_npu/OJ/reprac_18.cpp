//
// Created by 86180 on 25-9-18.
//
#include <iostream>
using namespace std;

int cross(int line,int crossline) {
    if (line==1||line==0) {
        return line+crossline;
    }
    return cross(line-1,crossline)+cross(line-1,crossline++);
}

int main()
{
    int getval;
    cin>>getval;
    int res = cross(getval,0);
    cout<<res;
    return 0;
}

