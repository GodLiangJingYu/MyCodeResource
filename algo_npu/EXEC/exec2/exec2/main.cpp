#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "Back4TSP.cpp"
#include "BB4TSP.cpp"

using namespace std;

// 随机生成 TSP 邻接矩阵
vector<vector<int>> generateRandomTSP(int n, int maxWeight = 100) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, maxWeight);

    vector<vector<int>> matrix(n + 1, vector<int>(n + 1, -1));
    for (int i = 1; i <= n; ++i) {
        matrix[i][i] = 0; // 对角线为 0
        for (int j = i + 1; j <= n; ++j) {
            matrix[i][j] = matrix[j][i] = dis(gen); // 对称边权
        }
    }
    return matrix;
}

int main() {
    vector<int> sizes = {10, 20, 40, 80, 100, 120, 160, 180, 200,500}; // 500 可能太慢，视情况添加
    for (int n : sizes) {
        cout << "Testing n = " << n << endl;
        auto matrix = generateRandomTSP(n);

        Back4TSP backSolver;
        BB4TSP bbSolver;

        // 测试回溯法
        auto start = chrono::high_resolution_clock::now();
        backSolver.backtrack4TSP(matrix, n);
        auto end = chrono::high_resolution_clock::now();
        auto backTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Backtrack: " << backTime << " ms, Cost = " << backSolver.getBestCost() << endl;

        // 测试分支定界法
        start = chrono::high_resolution_clock::now();
        bbSolver.bb4TSP(matrix, n);
        end = chrono::high_resolution_clock::now();
        auto bbTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Branch-and-Bound: " << bbTime << " ms, Cost = " << bbSolver.getMinCost() << endl;
    }
    return 0;
}