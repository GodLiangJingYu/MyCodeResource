//
// Created by 86180 on 25-9-16.
//
#include <iostream>
using namespace std;

int getres(int input) {
    int res= 0;

    for (int i=1;i<=input/2+1;i++) {
        int m=2*input%i;
        if (m==0) {
            int midnum=input/i;
            if ((input*2/i)%2==0) {
                if (midnum-i/2>0) {
                    res++;
                }
            }else {
                if (midnum-i/2+1>0) {
                    res++;
                }
            }
        }
    }
    return res;
}

int main() {
    int input,output;
    cin>>input;
    output = getres(input);
    cout<<output;
}