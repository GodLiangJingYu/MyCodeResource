//
// Created by 86180 on 25-9-23.
//
#include<algorithm>
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
double res=999999999;

struct point{           //记录点的坐标
    int x,y;
}p[200005];

double lengthPoint(int m,int n) {
    int x=p[m].x-p[n].x;
    int y=p[m].y-p[n].y;
    return sqrt(x*x+y*y);
}

double findMax() {

}

int main(){
    int n;
    scanf("%d",&n);
    for(int i = 1; i<=n; i++){
        scanf("%d%d",&p[i].x,&p[i].y);
    }
    printf("%0.6lf",res);
    return 0;
}