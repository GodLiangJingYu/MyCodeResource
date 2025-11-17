#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class BacktrackTSP {
private:
    int NoEdge = -1;
    int bigInt = INT_MAX;
    vector<vector<int>> a; // 邻接矩阵
    int cc = 0; // 存储当前费用
    int bestc = bigInt;// 当前最优费用
    vector<int> x; // 当前解
    vector<int> bestx;// 当前最优解
    int n = 0; // 城市数
    
    void swap(int i, int j) {
        int temp = x[i];
        x[i] = x[j];
        x[j] = temp;
    }
    
    bool check(int pos) {
        //TODO: 检查当前路径是否可行（剪枝函数）
        // 提示：检查从x[pos-1]到x[pos]的边是否存在，以及当前花费是否超过已知最优解
        // 步骤：1. 检查边是否存在 2. 计算当前路径花费 3. 与bestc比较
        // 返回值：如果路径可行且可能产生更优解则返回true，否则返回false
        
        // 示例实现：
        if (pos <= 1) return true;
        
        int from = x[pos - 1];
        int to = x[pos];
        
        // 检查边是否存在
        if (a[from - 1][to - 1] == NoEdge) {
            return false;
        }
        
        // 计算当前路径花费
        int currentCost = 0;
        for (int i = 1; i < pos; i++) {
            int cityFrom = x[i - 1];
            int cityTo = x[i];
            currentCost += a[cityFrom - 1][cityTo - 1];
        }
        currentCost += a[from - 1][to - 1];
        
        // 剪枝：如果当前花费已经超过最优解，返回false
        return currentCost < bestc;
    }
    
    void backtrack(int i) {
        if (i > n) {
            //TODO: 已经到达叶节点，需要计算当前路径的总花费
            // 提示：检查从最后一个城市回到起点的边是否存在，如果存在则更新最佳解
            // 步骤：1. 计算当前路径总花费 2. 与最佳解比较 3. 如果更优则更新bestc和bestx
            
            // 示例实现：
            int totalCost = 0;
            
            // 计算完整路径的花费
            for (int j = 1; j < n; j++) {
                int from = x[j - 1];
                int to = x[j];
                if (a[from - 1][to - 1] == NoEdge) {
                    return; // 路径不完整
                }
                totalCost += a[from - 1][to - 1];
            }
            
            // 检查回到起点的边
            int lastCity = x[n - 1];
            int firstCity = x[0];
            if (a[lastCity - 1][firstCity - 1] == NoEdge) {
                return; // 无法回到起点
            }
            totalCost += a[lastCity - 1][firstCity - 1];
            
            // 更新最优解
            if (totalCost < bestc) {
                bestc = totalCost;
                bestx = x;
            }
        } else {
            //TODO: 递归搜索所有可能的城市排列
            // 提示：对于当前层i，尝试所有未访问的城市，递归调用backtrack(i+1)
            // 步骤：1. 循环遍历城市j从i到n 2. 交换x[i]和x[j] 3. 检查是否可行 4. 递归调用 5. 回溯(再次交换)
            
            // 示例实现：
            for (int j = i; j <= n; j++) {
                swap(i, j);
                
                if (check(i)) {
                    backtrack(i + 1);
                }
                
                swap(i, j); // 回溯
            }
        }
    }
    
public:
    void backtrack4TSP(vector<vector<int>>& b, int num) {
        n = num;
        x.resize(n + 1);
        for (int i = 0; i <= n; i++)
            x[i] = i;
        bestx.resize(n + 1);
        a = b;
        backtrack(2);
    }
    
    int getBestCost() {
        return bestc;
    }
    
    vector<int> getBestPath() {
        return bestx;
    }
};

// 测试代码
int main() {
    BacktrackTSP solution;
    
    // 测试用例1：4个城市的完全图
    vector<vector<int>> graph1 = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    
    cout << "回溯法 - 测试用例1 - 4个城市：" << endl;
    cout << "城市间代价矩阵：" << endl;
    for (const auto& row : graph1) {
        for (int cost : row) {
            cout << cost << " ";
        }
        cout << endl;
    }
    
    solution.backtrack4TSP(graph1, 4);
    cout << "最小总代价: " << solution.getBestCost() << endl;
    
    vector<int> path = solution.getBestPath();
    cout << "最优路径: ";
    for (int city : path) {
        cout << city << " ";
    }
    cout << endl << endl;
    
    // 测试用例2：3个城市的简单图
    vector<vector<int>> graph2 = {
        {0, 5, 8},
        {5, 0, 6},
        {8, 6, 0}
    };
    
    cout << "回溯法 - 测试用例2 - 3个城市：" << endl;
    solution.backtrack4TSP(graph2, 3);
    cout << "最小总代价: " << solution.getBestCost() << endl;
    
    path = solution.getBestPath();
    cout << "最优路径: ";
    for (int city : path) {
        cout << city << " ";
    }
    cout << endl;
    
    return 0;
}