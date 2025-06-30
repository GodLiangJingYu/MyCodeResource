//
// Created by 86180 on 25-6-30.
//
#include <algorithm>
#include <deque>
#include<iostream>
#include <unordered_map>
#include <vector>
using namespace std;

int main() {

    //初始化，int是key，string是value
    unordered_map<int,string> map;

    //在哈希表中添加
    map[12345] = "john";
    map[66666] = "jee";

    //遍历哈希表
    //遍历键值对
    for (auto kv: map) {
        cout << kv.first << "->" << kv.second << endl;
    }

    //使用迭代器遍历键值对
    for (auto iter = map.begin();iter != map.end();iter++) {
        cout<< iter->first << "->" << iter->second << endl;
    }

    cout<<map[12345]<<endl;

    return 0;
}