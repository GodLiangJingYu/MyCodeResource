#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

using namespace std;

void do_it(int x1,int y1,int x2,int y2,int x3,int y3,vector<vector<int>> &res,int num) {
    res[x1][y1]=num;
    res[x2][y2]=num;
    res[x3][y3]=num;
    return;
}

void do_loop(vector<vector<int>> &res,int x,int y,int num,int midx,int midy,int len) {
    /*
     * 判断，回溯
     */
    if (x==midx&&y==midy) {
        return;
    }
    if (x<=midx&&y<=midy) {
        do_it(midx+1,midy+1,midx+1,midy,midx,midy+1,res,num);
        do_loop(res,midx+1,midy,++num,midx+len,midy-len,len/4);
        do_loop(res,midx,midy+1,++num,midx-len,midy+len,len/4);
        do_loop(res,midx+1,midy+1,++num,midx+len,midy+len,len/4);
    }else if (x>midx&&y<=midy) {
        do_it(midx,midy,midx,midy+1,midx+1,midy+1,res,num);
        do_loop(res,midx,midy,++num,midx-len,midy-len,len/4);
        do_loop(res,midx,midy+1,++num,midx-len,midy+len,len/4);
        do_loop(res,midx+1,midy+1,++num,midx+len,midy+len,len/4);
    }else if (x<=midx&&y>midy) {
        do_it(midx,midy,midx,midy+1,midx+1,midy+1,res,num);
        do_loop(res,midx,midy,++num,midx-len,midy-len,len/4);
        do_loop(res,midx+1,midy,++num,midx+len,midy-len,len/4);
        do_loop(res,midx+1,midy+1,++num,midx+len,midy+len,len/4);
    }else {
        do_it(midx,midy,midx,midy+1,midx+1,midy,res,num);
        do_loop(res,midx,midy,++num,midx-len,midy-len,len/4);
        do_loop(res,midx,midy+1,++num,midx-len,midy+len,len/4);
        do_loop(res,midx+1,midy,++num,midx+len,midy-len,len/4);
    }

}

void num_size(vector<vector<int>> &res,int size) {
    /*
     * 对得出的L进行编号
     */
}


int main() {
    int size_loop,x,y;
    int size=1;
    cin>>size_loop>>x>>y;
    for (int i=0;i<size_loop;i++) {
        size*=2;
    }
    vector<vector<int>> res(size);
    res[x][y]=0;
    do_loop(res,x,y,1,size/2,size/2,size/4);
    //num_size(res,size);
    for (int i=0;i<size;i++) {
        for (int j=0;j<size;j++) {
            cout<<res[i][j]<<' ';
            if (j==size-1) {
                cout<<endl;
            }
        }
    }
    return 0;
}
