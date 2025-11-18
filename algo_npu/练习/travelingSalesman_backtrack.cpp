#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class BacktrackTSP {
private:
    vector<vector<int>> graph; // 邻接矩阵
    vector<int> path;          // 当前路径
    vector<int> bestPath;      // 最优路径
    vector<bool> visited;      // 访问标记
    int minCost;               // 最小代价
    int n;                     // 城市数量
    
    void backtrack(int currentCity, int count, int currentCost) {
        // 如果访问了所有城市
        if (count == n) {
            // 检查能否回到起点
            if (graph[currentCity][0] != -1) {
                int totalCost = currentCost + graph[currentCity][0];
                if (totalCost < minCost) {
                    minCost = totalCost;
                    bestPath = path;
                }
            }
            return;
        }
        
        // 尝试访问下一个未访问的城市
        for (int nextCity = 0; nextCity < n; nextCity++) {
            if (!visited[nextCity] && graph[currentCity][nextCity] != -1) {
                // 剪枝：如果当前代价已经超过最小代价，跳过
                if (currentCost + graph[currentCity][nextCity] >= minCost) {
                    continue;
                }
                
                // 访问下一个城市
                visited[nextCity] = true;
                path.push_back(nextCity);
                
                // 递归
                backtrack(nextCity, count + 1, currentCost + graph[currentCity][nextCity]);
                
                // 回溯
                path.pop_back();
                visited[nextCity] = false;
            }
        }
    }
    
public:
    void backtrack4TSP(vector<vector<int>>& g, int num) {
        graph = g;
        n = num;
        minCost = INT_MAX;
        
        visited.resize(n, false);
        path.clear();
        bestPath.clear();
        
        // 从城市0开始
        visited[0] = true;
        path.push_back(0);
        
        backtrack(0, 1, 0);
    }
    
    int getBestCost() {
        return minCost;
    }
    
    vector<int> getBestPath() {
        return bestPath;
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
        cout << city + 1 << " "; // 转换为1-based显示
    }
    cout << "1" << endl; // 回到起点
    cout << endl;
    
    // 测试用例2：3个城市的简单图
    vector<vector<int>> graph2 = {
        {0, 5, 8},
        {5, 0, 6},
        {8, 6, 0}
    };
    
    cout << "回溯法 - 测试用例2 - 3个城市：" << endl;
    cout << "城市间代价矩阵：" << endl;
    for (const auto& row : graph2) {
        for (int cost : row) {
            cout << cost << " ";
        }
        cout << endl;
    }
    
    solution.backtrack4TSP(graph2, 3);
    cout << "最小总代价: " << solution.getBestCost() << endl;
    
    path = solution.getBestPath();
    cout << "最优路径: ";
    for (int city : path) {
        cout << city + 1 << " "; // 转换为1-based显示
    }
    cout << "1" << endl; // 回到起点
    cout << endl;
    
    return 0;
}