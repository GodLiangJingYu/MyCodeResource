//
// Created by 86180 on 25-10-24.
//
#include <vector>
#include <climits>
#include <queue>

using namespace std;

class BB4TSP {
private:
    const int NoEdge = -1;
    int minCost = INT_MAX; // 当前最小路径长度
    vector<int> bestH; // 最优路径
    struct HeapNode {
        vector<int> liveNode; // 活节点（当前路径）
        int lcost; // 下界
        int level; // 当前层级（已确定的城市数）

        HeapNode(const vector<int>& node, int lb, int lev) : liveNode(node), lcost(lb), level(lev) {}

        // 用于优先队列比较（最小堆）
        bool operator>(const HeapNode& other) const {
            return lcost > other.lcost;
        }
    };
    priority_queue<HeapNode, vector<HeapNode>, greater<HeapNode>> heap; // 优先队列存储活节点

    int computeLB(const vector<int>& liveNode, int level, const vector<vector<int>>& cMatrix) {
        // TODO: 实现下界函数，计算当前活节点的下界
        // 例如：计算当前路径长度 + 剩余城市的最小出边和入边之和
        return -1;
    }

public:
    int bb4TSP(const vector<vector<int>>& cMatrix, int n) {
        // 初始化根节点
        vector<int> liveNode;
        for (int i = 1; i <= n; ++i) {
            liveNode.push_back(i);
        }
        int level = 1; // 当前层级
        int lcost = computeLB(liveNode, level, cMatrix); // 计算初始下界
        heap.push(HeapNode(liveNode, lcost, level));

        while (!heap.empty()) {
            // TODO: 从优先队列中取出下界最小的节点，扩展子节点，并更新 minCost 和 bestH
            // 例如：扩展每个未访问城市，计算新下界，若小于 minCost 则加入队列
        }

        return minCost;
    }

    int getMinCost() const {
        return minCost;
    }

    vector<int> getBestPath() const {
        return bestH;
    }
};