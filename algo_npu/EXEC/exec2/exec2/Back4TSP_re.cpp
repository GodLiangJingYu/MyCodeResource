//
// Created by 86180 on 25-10-24.
//
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

class Back4TSP {
private:
    const int NoEdge = -1;
    const int bigInt = INT_MAX;
    vector<vector<int>> a; // 邻接矩阵
    int cc = 0; // 当前路径长度
    int bestc = bigInt; // 当前最优路径长度
    vector<int> x; // 当前路径
    vector<int> bestx; // 当前最优路径
    int n = 0; // 城市数量

    void backtrack(int i) {
        if (i > n) {
            // TODO: 当到达叶子节点（所有城市都访问过）时，检查是否形成有效路径并更新最优解

        } else {
            // TODO: 遍历可能的下一个城市，生成子节点并递归

        }
    }

    void swap(int i, int j) {
        int temp = x[i];
        x[i] = x[j];
        x[j] = temp;
    }

    bool check(int pos) {
        // TODO: 检查当前城市 pos 是否可以加入路径
        // 例如：检查 a[x[i-1]][pos] 是否为 NoEdge，是否已访问
        return false;
    }

public:
    void backtrack4TSP(const vector<vector<int>>& b, int num) {
        n = num;
        x.resize(n + 1);
        for (int i = 0; i <= n; ++i) {
            x[i] = i;
        }
        bestx.resize(n + 1);
        a = b;
        backtrack(2);
    }

    int getBestCost() const {
        return bestc;
    }

    vector<int> getBestPath() const {
        return bestx;
    }
};