#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// 集装箱结构体
struct Container {
    int id;     // 集装箱编号
    int weight; // 集装箱重量
};

// 结点结构体（用于分支限界）
struct Node {
    int cw;              // 当前装载重量
    int level;           // 当前层数
    vector<int> path;    // 当前路径（装载的集装箱编号）
    int ub;              // 上界（用于剪枝）
    
    Node(int w, int l) : cw(w), level(l) {}
};

// 比较函数，按重量降序排列（用于剪枝优化）
bool compareContainers(const Container& a, const Container& b) {
    return a.weight > b.weight;
}

class LoadingProblem {
private:
    int n;                           // 集装箱数量
    int C1, C2;                      // 两艘货轮的载重量
    vector<Container> containers;    // 集装箱数组
    int totalWeight;                 // 所有集装箱总重量
    int bestWeight;                  // 第一艘货轮的最优装载重量
    vector<int> bestPath;            // 最优装载方案
    
public:
    // 构造函数
    LoadingProblem(int n, int c1, int c2, vector<int> weights) 
        : n(n), C1(c1), C2(c2), totalWeight(0), bestWeight(0) {
        
        // 初始化集装箱数组
        for (int i = 0; i < n; i++) {
            containers.push_back({i + 1, weights[i]});
            totalWeight += weights[i];
        }
        
        // 按重量降序排列（优化剪枝效果）
        sort(containers.begin(), containers.end(), compareContainers);
    }
    
    // 检查是否存在装载方案
    bool hasSolution() {
        return totalWeight <= C1 + C2;
    }
    
    // 计算上界函数
    int calculateUpperBound(int cw, int level) {
        int ub = cw;
        for (int i = level; i < n; i++) {
            ub += containers[i].weight;
        }
        return min(ub, C1); // 不能超过第一艘货轮的载重量
    }
    
    // FIFO队列式分支限界算法
    void solveLoadingProblem() {

    }
    
    // 输出结果
    void printResult() {
        if (!hasSolution()) {
            cout << "No" << endl;
            return;
        }
        
        cout << "Yes" << endl;
        cout << bestWeight << endl;
        
        // 输出装载方案
        for (size_t i = 0; i < bestPath.size(); i++) {
            if (i > 0) cout << " ";
            cout << bestPath[i];
        }
        cout << endl;
    }
};

int main() {
    int n, C1, C2;
    
    // 输入集装箱数量和两艘货轮的载重量
    cin >> n >> C1 >> C2;
    
    // 输入集装箱重量
    vector<int> weights(n);
    for (int i = 0; i < n; i++) {
        cin >> weights[i];
    }
    
    // 创建装载问题实例
    LoadingProblem problem(n, C1, C2, weights);
    
    // 检查是否有解
    if (!problem.hasSolution()) {
        cout << "No" << endl;
        return 0;
    }
    
    // 解决装载问题
    problem.solveLoadingProblem();
    
    // 输出结果
    problem.printResult();
    
    return 0;
}