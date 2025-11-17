#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

class BranchBoundTSP {
private:
    int NoEdge = -1; // 表示没有边
    int minCost = INT_MAX; // 当前最小费用
    vector<int> bestH; // 最优路径
    
    struct HeapNode {
        vector<int> liveNode; // 活结点
        int lcost; // 费用的下界
        int level; // 0-level的层数已经排好的
        
        HeapNode(vector<int> node, int lb, int lev) {
            liveNode = node;
            lcost = lb;
            level = lev;
        }
        
        // 优先队列的比较函数
        bool operator<(const HeapNode& other) const {
            return lcost > other.lcost; // 小顶堆，下界小的优先
        }
    };
    
    int computeLB(vector<int>& liveNode, int level, vector<vector<int>>& cMatrix) {
        //TODO: 计算当前部分路径的下界（Lower Bound）
        // 提示：使用最小生成树或简化方法计算下界，用于剪枝
        // 步骤：1. 计算已确定路径的花费 2. 估算剩余路径的最小可能花费 3. 返回总和作为下界
        // 注意：下界应该小于等于实际最优解，确保不会剪掉最优解
        
        // 示例实现：简化版本，计算已确定路径花费 + 剩余城市最小出边之和
        if (liveNode.empty()) return 0;
        
        int n = cMatrix.size();
        int currentCost = 0;
        
        // 计算已确定路径的花费
        for (int i = 0; i < level - 1; i++) {
            int from = liveNode[i] - 1;
            int to = liveNode[i + 1] - 1;
            if (cMatrix[from][to] == NoEdge) {
                return INT_MAX; // 路径不完整
            }
            currentCost += cMatrix[from][to];
        }
        
        // 计算剩余城市的最小出边之和（简化下界）
        int remainingCost = 0;
        vector<bool> visited(n, false);
        
        // 标记已访问的城市
        for (int i = 0; i < level; i++) {
            visited[liveNode[i] - 1] = true;
        }
        
        // 对每个未访问城市，找到最小的出边
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                int minEdge = INT_MAX;
                for (int j = 0; j < n; j++) {
                    if (i != j && cMatrix[i][j] != NoEdge && cMatrix[i][j] < minEdge) {
                        minEdge = cMatrix[i][j];
                    }
                }
                if (minEdge != INT_MAX) {
                    remainingCost += minEdge;
                }
            }
        }
        
        return currentCost + remainingCost;
    }
    
public:
    int bb4TSP(vector<vector<int>>& cMatrix, int n) {
        // 创建初始节点
        vector<int> liveNode;
        for (int i = 1; i <= n; i++) liveNode.push_back(i);
        
        int level = 1; // 0-level的层数已经排好的
        int lcost = computeLB(liveNode, level, cMatrix); // 费用的下界
        
        priority_queue<HeapNode> pq;
        pq.push(HeapNode(liveNode, lcost, level));
        
        while (!pq.empty()) {
            HeapNode current = pq.top();
            pq.pop();
            
            if (current.level == n) {
                // 到达叶节点，检查完整路径
                int totalCost = 0;
                bool valid = true;
                
                // 计算完整路径花费
                for (int i = 0; i < n - 1; i++) {
                    int from = current.liveNode[i] - 1;
                    int to = current.liveNode[i + 1] - 1;
                    if (cMatrix[from][to] == NoEdge) {
                        valid = false;
                        break;
                    }
                    totalCost += cMatrix[from][to];
                }
                
                // 检查回到起点
                if (valid) {
                    int last = current.liveNode[n - 1] - 1;
                    int first = current.liveNode[0] - 1;
                    if (cMatrix[last][first] != NoEdge) {
                        totalCost += cMatrix[last][first];
                        if (totalCost < minCost) {
                            minCost = totalCost;
                            bestH = current.liveNode;
                        }
                    }
                }
                continue;
            }
            
            //TODO: 分支限界法主循环
            // 提示：使用优先队列存储活节点，每次选择下界最小的节点扩展
            // 步骤：1. 从当前节点生成子节点 2. 计算每个子节点的下界 3. 加入优先队列 4. 选择最优节点继续扩展
            
            // 示例实现：生成子节点
            for (int i = current.level; i < n; i++) {
                vector<int> newNode = current.liveNode;
                swap(newNode[current.level], newNode[i]);
                
                int newLevel = current.level + 1;
                int newLcost = computeLB(newNode, newLevel, cMatrix);
                
                // 剪枝：如果下界小于当前最优解，则加入队列
                if (newLcost < minCost) {
                    pq.push(HeapNode(newNode, newLcost, newLevel));
                }
            }
        }
        
        return minCost;
    }
    
    vector<int> getBestPath() {
        return bestH;
    }
};

// 测试代码
int main() {
    BranchBoundTSP solution;
    
    // 测试用例1：4个城市的完全图
    vector<vector<int>> graph1 = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    
    cout << "分支限界法 - 测试用例1 - 4个城市：" << endl;
    cout << "城市间代价矩阵：" << endl;
    for (const auto& row : graph1) {
        for (int cost : row) {
            cout << cost << " ";
        }
        cout << endl;
    }
    
    int result1 = solution.bb4TSP(graph1, 4);
    cout << "最小总代价: " << result1 << endl;
    
    vector<int> path1 = solution.getBestPath();
    cout << "最优路径: ";
    for (int city : path1) {
        cout << city << " ";
    }
    cout << endl << endl;
    
    // 测试用例2：3个城市的简单图
    vector<vector<int>> graph2 = {
        {0, 5, 8},
        {5, 0, 6},
        {8, 6, 0}
    };
    
    cout << "分支限界法 - 测试用例2 - 3个城市：" << endl;
    int result2 = solution.bb4TSP(graph2, 3);
    cout << "最小总代价: " << result2 << endl;
    
    vector<int> path2 = solution.getBestPath();
    cout << "最优路径: ";
    for (int city : path2) {
        cout << city << " ";
    }
    cout << endl;
    
    return 0;
}