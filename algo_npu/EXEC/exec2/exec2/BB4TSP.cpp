//
// Created by 86180 on 25-10-24.
//
//
// Created by 86180 on 25-10-24.
//
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>

using namespace std;

class BB4TSP {
private:
    const int NoEdge = -1;
    int minCost = INT_MAX; // 当前最小路径长度
    vector<int> bestH; // 最优路径
    long long nodeCount = 0; // 搜索节点计数
    int boundType = 1; // 界值函数类型

    struct HeapNode {
        vector<int> liveNode; // 活节点（当前路径）
        int lcost; // 下界
        int level; // 当前层级（已确定的城市数）
        int currentCost; // 当前实际路径长度

        HeapNode(const vector<int>& node, int lb, int lev, int cc)
            : liveNode(node), lcost(lb), level(lev), currentCost(cc) {}

        // 用于优先队列比较（最小堆）
        bool operator>(const HeapNode& other) const {
            return lcost > other.lcost;
        }
    };
    priority_queue<HeapNode, vector<HeapNode>, greater<HeapNode>> heap; // 优先队列存储活节点

    // 界值函数1：基于最小生成树的下界
    int computeLB1(const vector<int>& path, int level, int currentCost, const vector<vector<int>>& cMatrix, int n) {
        if (level == 1) {
            return 0;
        }

        int bound = currentCost;
        vector<bool> visited(n + 1, false);
        for (int i = 0; i < level; ++i) {
            visited[path[i]] = true;
        }

        // 计算未访问城市的最小边权和
        int minSum = 0;
        for (int i = 1; i <= n; ++i) {
            if (!visited[i]) {
                int minEdge = INT_MAX;
                for (int j = 1; j <= n; ++j) {
                    if (i != j && cMatrix[i][j] != NoEdge) {
                        minEdge = min(minEdge, cMatrix[i][j]);
                    }
                }
                if (minEdge != INT_MAX) {
                    minSum += minEdge;
                }
            }
        }

        // 添加回到起点的边的估计
        int minToStart = INT_MAX;
        for (int i = 1; i <= n; ++i) {
            if (!visited[i] && cMatrix[i][path[0]] != NoEdge) {
                minToStart = min(minToStart, cMatrix[i][path[0]]);
            }
        }

        if (minToStart != INT_MAX) {
            minSum += minToStart;
        }

        return bound + minSum;
    }

    // 界值函数2：基于每个城市的最小出边和入边
    int computeLB2(const vector<int>& path, int level, int currentCost, const vector<vector<int>>& cMatrix, int n) {
        if (level == 1) {
            return 0;
        }

        vector<bool> visited(n + 1, false);
        for (int i = 0; i < level; ++i) {
            visited[path[i]] = true;
        }

        int bound = currentCost;

        // 对于每个未访问城市，计算最小出边和入边
        for (int i = 1; i <= n; ++i) {
            if (!visited[i]) {
                int minOut = INT_MAX;
                int minIn = INT_MAX;

                for (int j = 1; j <= n; ++j) {
                    if (i != j) {
                        if (cMatrix[i][j] != NoEdge) {
                            minOut = min(minOut, cMatrix[i][j]);
                        }
                        if (cMatrix[j][i] != NoEdge) {
                            minIn = min(minIn, cMatrix[j][i]);
                        }
                    }
                }

                if (minOut != INT_MAX) {
                    bound += minOut / 2;
                }
                if (minIn != INT_MAX) {
                    bound += minIn / 2;
                }
            }
        }

        return bound;
    }

    // 界值函数3：改进的下界，考虑当前位置
    int computeLB3(const vector<int>& path, int level, int currentCost, const vector<vector<int>>& cMatrix, int n) {
        if (level == 1) {
            return 0;
        }

        vector<bool> visited(n + 1, false);
        for (int i = 0; i < level; ++i) {
            visited[path[i]] = true;
        }

        int bound = currentCost;
        int lastCity = path[level - 1];

        // 从当前城市到未访问城市的最小边
        int minFromCurrent = INT_MAX;
        for (int i = 1; i <= n; ++i) {
            if (!visited[i] && cMatrix[lastCity][i] != NoEdge) {
                minFromCurrent = min(minFromCurrent, cMatrix[lastCity][i]);
            }
        }

        if (minFromCurrent != INT_MAX) {
            bound += minFromCurrent;
        }

        // 未访问城市之间的最小边权和
        vector<int> unvisited;
        for (int i = 1; i <= n; ++i) {
            if (!visited[i]) {
                unvisited.push_back(i);
            }
        }

        for (int city : unvisited) {
            int minEdge = INT_MAX;
            for (int j = 1; j <= n; ++j) {
                if (city != j && cMatrix[city][j] != NoEdge) {
                    minEdge = min(minEdge, cMatrix[city][j]);
                }
            }
            if (minEdge != INT_MAX) {
                bound += minEdge / unvisited.size();
            }
        }

        return bound;
    }

    int computeLB(const vector<int>& path, int level, int currentCost, const vector<vector<int>>& cMatrix, int n) {
        switch (boundType) {
            case 1:
                return computeLB1(path, level, currentCost, cMatrix, n);
            case 2:
                return computeLB2(path, level, currentCost, cMatrix, n);
            case 3:
                return computeLB3(path, level, currentCost, cMatrix, n);
            default:
                return computeLB1(path, level, currentCost, cMatrix, n);
        }
    }

public:
    void setBoundType(int type) {
        boundType = type;
    }

    int bb4TSP(const vector<vector<int>>& cMatrix, int n) {
        minCost = INT_MAX;
        bestH.clear();
        nodeCount = 0;

        // 清空优先队列
        while (!heap.empty()) {
            heap.pop();
        }

        // 初始化根节点（从城市1开始）
        vector<int> liveNode;
        liveNode.push_back(1);
        int level = 1;
        int lcost = computeLB(liveNode, level, 0, cMatrix, n);
        heap.push(HeapNode(liveNode, lcost, level, 0));

        while (!heap.empty()) {
            nodeCount++;
            HeapNode current = heap.top();
            heap.pop();

            // 剪枝：如果当前下界已经大于等于已知最优解，跳过
            if (current.lcost >= minCost) {
                continue;
            }

            // 如果已经访问了所有城市
            if (current.level == n) {
                // 检查能否回到起点
                int lastCity = current.liveNode[n - 1];
                if (cMatrix[lastCity][1] != NoEdge) {
                    int totalCost = current.currentCost + cMatrix[lastCity][1];
                    if (totalCost < minCost) {
                        minCost = totalCost;
                        bestH = current.liveNode;
                    }
                }
                continue;
            }

            // 扩展子节点
            int lastCity = current.liveNode[current.level - 1];
            vector<bool> visited(n + 1, false);
            for (int city : current.liveNode) {
                visited[city] = true;
            }

            for (int nextCity = 1; nextCity <= n; ++nextCity) {
                if (!visited[nextCity] && cMatrix[lastCity][nextCity] != NoEdge) {
                    vector<int> newPath = current.liveNode;
                    newPath.push_back(nextCity);
                    int newCost = current.currentCost + cMatrix[lastCity][nextCity];
                    int newLevel = current.level + 1;
                    int newLB = computeLB(newPath, newLevel, newCost, cMatrix, n);

                    // 只有当下界小于当前最优解时才加入队列
                    if (newLB < minCost) {
                        heap.push(HeapNode(newPath, newLB, newLevel, newCost));
                    }
                }
            }
        }

        return minCost;
    }

    int getMinCost() const {
        return minCost;
    }

    vector<int> getBestPath() const {
        return bestH;
    }

    long long getNodeCount() const {
        return nodeCount;
    }
};