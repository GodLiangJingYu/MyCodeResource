//
// Created by 86180 on 25-6-21.
//
#include<stdio.h>

int main() {
    int n,num,i=1,add=2;
    scanf("%d",&n);
    while(i<n){
        add+=i;
        i++;
    }
    printf("%d",add)
    return 0;
}
