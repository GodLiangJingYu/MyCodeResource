//
// Created by 86180 on 25-9-22.
//
#include<iostream>
#include<cstdio>
#include<algorithm>
using namespace std;
#define N 10001
#define re register

inline int read(){
    int x=0,f=1;
    char c=getchar();
    while(c<'0'||c>'9'){
        if(c=='-')f=-1;
        c=getchar();
    }
    while(c>='0'&&c<='9'){
        x=(x<<3)+(x<<1)+c-'0';
        c=getchar();
    }
    return x*f;
}



int main(){
    int n=read(),m=read();
    for(re int i=1;i<=n-1;i++){
        int u,v,w;
        u=read(),v=read(),w=read();
        add(u,v,w);
        add(v,u,w);
    }
    for(re int i=1;i<=m;i++){
        query[i]=read();
        if(!query[i])ok[i]=1;//这里,加个特判
    }
    for(re int i=1;i<=m;i++){
        if(ok[i]){
            cout<<"AYE"<<endl;
        }
        else{
            cout<<"NAY"<<endl;
        }
    }
    return 0;
}