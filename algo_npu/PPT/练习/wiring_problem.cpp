#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// 方向结构体
struct Direction {
    int dr, dc;  // 行和列的偏移量
};

// 位置结构体
struct Position {
    int row, col;
    
    Position(int r = 0, int c = 0) : row(r), col(c) {}
    
    // 重载==运算符用于比较位置
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

// 结点结构体（用于分支限界）
struct Node {
    Position pos;      // 当前位置
    int distance;      // 从起点到当前位置的距离
    vector<Position> path;  // 路径
    
    Node(Position p, int d) : pos(p), distance(d) {}
};

class WiringProblem {
private:
    int size;                        // 布线区域大小 (size x size)
    vector<vector<int>> grid;        // 布线网格 (0:空, 1:障碍, -1:已访问)
    Position start, finish;          // 起点和终点位置
    vector<Position> shortestPath;   // 最短路径
    bool found;                      // 是否找到路径
    
    // 四个移动方向：右、下、左、上
    static const Direction directions[4];
    
public:
    // 构造函数
    WiringProblem(int n) : size(n), found(false) {
        grid.resize(size + 2, vector<int>(size + 2, 0));
    }
    
    // 设置边界围墙
    void setBoundary() {
        for (int i = 0; i <= size + 1; i++) {
            grid[0][i] = 1;           // 上边界
            grid[size + 1][i] = 1;    // 下边界
            grid[i][0] = 1;           // 左边界
            grid[i][size + 1] = 1;    // 右边界
        }
    }
    
    // 设置障碍物
    void setObstacle(int row, int col) {
        if (row >= 1 && row <= size && col >= 1 && col <= size) {
            grid[row][col] = 1;
        }
    }
    
    // 设置起点和终点
    void setStartEnd(Position s, Position e) {
        start = s;
        finish = e;
    }
    
    // 检查位置是否有效且未访问
    bool isValidPosition(const Position& pos) {
        return pos.row >= 1 && pos.row <= size && 
               pos.col >= 1 && pos.col <= size && 
               grid[pos.row][pos.col] == 0;
    }
    
    // 检查是否到达终点
    bool isFinish(const Position& pos) {
        return pos == finish;
    }
    
    // FIFO队列式分支限界算法
    void solveWiringProblem() {
        // TODO: 请在这里实现FIFO队列式分支限界算法
        // 提示：
        // 1. 初始化队列，加入起点
        // 2. 标记起点为已访问
        // 3. 循环处理队列中的结点
        // 4. 对每个结点，向四个方向扩展
        // 5. 检查新位置是否有效
        // 6. 如果到达终点，记录路径并返回
        // 7. 否则将新位置加入队列
        // 8. 如果队列为空且未找到终点，说明无解
    }
    
    // 输出结果
    void printResult() {
        if (!found) {
            cout << "无法找到从起点到终点的路径" << endl;
            return;
        }
        
        cout << "找到最短路径，长度为: " << shortestPath.size() - 1 << endl;
        cout << "路径: ";
        for (size_t i = 0; i < shortestPath.size(); i++) {
            if (i > 0) cout << " → ";
            cout << "(" << shortestPath[i].row << "," << shortestPath[i].col << ")";
        }
        cout << endl;
        
        // 输出布线图
        printGrid();
    }
    
    // 输出布线图
    void printGrid() {
        cout << "\n布线图 (S=起点, E=终点, #=障碍, *=路径):\n";
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                Position current(i, j);
                if (current == start) {
                    cout << "S ";
                } else if (current == finish) {
                    cout << "E ";
                } else if (grid[i][j] == 1) {
                    cout << "# ";
                } else if (find(shortestPath.begin(), shortestPath.end(), current) != shortestPath.end()) {
                    cout << "* ";
                } else {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }
    
    // 优先队列式分支限界算法（使用A*思想）
    void solveWiringProblemAStar() {
        // TODO: 可选实现A*算法
        // 提示：使用启发式函数 f(n) = g(n) + h(n)
        // g(n): 从起点到当前点的实际距离
        // h(n): 从当前点到终点的估计距离（曼哈顿距离）
    }
};

// 初始化静态成员
const Direction WiringProblem::directions[4] = {
    {0, 1},   // 右
    {1, 0},   // 下
    {0, -1},  // 左
    {-1, 0}   // 上
};

int main() {
    int size;
    cout << "请输入布线区域大小: ";
    cin >> size;
    
    WiringProblem problem(size);
    problem.setBoundary();
    
    // 设置障碍物
    int numObstacles;
    cout << "请输入障碍物数量: ";
    cin >> numObstacles;
    
    cout << "请输入障碍物位置 (行 列):" << endl;
    for (int i = 0; i < numObstacles; i++) {
        int row, col;
        cin >> row >> col;
        problem.setObstacle(row, col);
    }
    
    // 设置起点和终点
    int sr, sc, fr, fc;
    cout << "请输入起点位置 (行 列): ";
    cin >> sr >> sc;
    cout << "请输入终点位置 (行 列): ";
    cin >> fr >> fc;
    
    problem.setStartEnd(Position(sr, sc), Position(fr, fc));
    
    // 解决布线问题
    problem.solveWiringProblem();
    
    // 输出结果
    problem.printResult();
    
    return 0;
}