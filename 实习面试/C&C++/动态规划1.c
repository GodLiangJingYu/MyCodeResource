//
// Created by 86180 on 25-6-21.
//
#include<stdio.h>

int main() {
    int n,num,i=0,add=2;
    scanf("%d",&n);
    if(n==1){
        printf("1");
        return 0;
    }
    while(i<n-1){
        add+=i;
        i++;
    }
    printf("%d",add);
    return 0;
}
