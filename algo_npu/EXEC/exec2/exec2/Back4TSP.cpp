//
// Created by 86180 on 25-10-24.
//
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
    long long nodeCount = 0; // 搜索节点计数

    // 计算从当前位置到未访问城市的最小边界（简单界值函数）
    int computeLowerBound1(int i) {
        int minSum = 0;
        // 计算剩余未访问城市之间的最小边权和
        for (int j = i; j <= n; ++j) {
            int minEdge = bigInt;
            for (int k = i; k <= n; ++k) {
                if (j != k && a[x[j]][x[k]] != NoEdge) {
                    int edgeWeight = a[x[j]][x[k]];
                    minEdge = min(minEdge, edgeWeight);
                }
            }
            if (minEdge != bigInt) {
                minSum += minEdge;
            }
        }
        return minSum;
    }

    // 改进的界值函数：考虑每个城市的最小出边
    int computeLowerBound2(int i) {
        int bound = cc;
        vector<bool> visited(n + 1, false);
        for (int j = 1; j < i; ++j) {
            visited[x[j]] = true;
        }

        // 对于每个未访问的城市，找最小出边
        for (int j = i; j <= n; ++j) {
            int minEdge = bigInt;
            for (int k = 1; k <= n; ++k) {
                if (!visited[k]) {
                    int cityJ = x[j];
                    int edgeWeight = a[cityJ][k];
                    if (edgeWeight != NoEdge) {
                        minEdge = min(minEdge, edgeWeight);
                    }
                }
            }
            if (minEdge != bigInt) {
                bound += minEdge;
            }
        }

        return bound;
    }

    void backtrack(int i) {
        nodeCount++;
        if (i > n) {
            // 到达叶子节点，检查是否能回到起点
            if (a[x[n]][x[1]] != NoEdge) {
                int totalCost = cc + a[x[n]][x[1]];
                if (totalCost < bestc) {
                    bestc = totalCost;
                    bestx = x;
                }
            }
        } else {
            // 遍历可能的下一个城市
            for (int j = i; j <= n; ++j) {
                // 检查是否可以到达城市 x[j]
                int prevCity = x[i - 1];
                int currCity = x[j];
                if (a[prevCity][currCity] != NoEdge) {
                    int edgeWeight = a[prevCity][currCity];
                    int newCost = cc + edgeWeight;

                    // 剪枝：如果当前路径已经超过最优解，则不继续
                    if (newCost < bestc) {
                        swap(i, j);
                        cc = newCost;
                        backtrack(i + 1);
                        cc = newCost - edgeWeight;
                        swap(i, j); // 回溯
                    }
                }
            }
        }
    }

    void swap(int i, int j) {
        int temp = x[i];
        x[i] = x[j];
        x[j] = temp;
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
        cc = 0;
        bestc = bigInt;
        nodeCount = 0;
        backtrack(2);
    }

    int getBestCost() const {
        return bestc;
    }

    vector<int> getBestPath() const {
        return bestx;
    }

    long long getNodeCount() const {
        return nodeCount;
    }
};