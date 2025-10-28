//
// Created by 86180 on 25-10-24.
//
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include "Back4TSP.cpp"
#include "BB4TSP.cpp"

using namespace std;

// Generate random TSP adjacency matrix
vector<vector<int>> generateRandomTSP(int n, int maxWeight = 100) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, maxWeight);

    vector<vector<int>> matrix(n + 1, vector<int>(n + 1, -1));
    for (int i = 1; i <= n; ++i) {
        matrix[i][i] = 0; // Diagonal is 0
        for (int j = i + 1; j <= n; ++j) {
            matrix[i][j] = matrix[j][i] = dis(gen); // Symmetric edge weights
        }
    }
    return matrix;
}

int main() {
    vector<int> sizes = {10, 20, 40, 80, 100, 120, 160, 180, 200}; // Test sizes

    cout << "========================================" << endl;
    cout << "TSP Algorithm Performance Comparison" << endl;
    cout << "Backtracking vs Branch-and-Bound" << endl;
    cout << "========================================" << endl << endl;

    for (int n : sizes) {
        cout << "========================================" << endl;
        cout << "Testing with n = " << n << " cities" << endl;
        cout << "========================================" << endl;

        auto matrix = generateRandomTSP(n);

        // Test Backtracking
        cout << "\n[1] Backtracking Algorithm:" << endl;
        Back4TSP backSolver;
        auto start = chrono::high_resolution_clock::now();
        backSolver.backtrack4TSP(matrix, n);
        auto end = chrono::high_resolution_clock::now();
        auto backTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "   Time: " << backTime << " ms" << endl;
        cout << "   Best Cost: " << backSolver.getBestCost() << endl;
        cout << "   Nodes Explored: " << backSolver.getNodeCount() << endl;

        // Test Branch-and-Bound - Bound Function 1
        cout << "\n[2] Branch-and-Bound (Bound Function 1 - MST Based):" << endl;
        BB4TSP bbSolver1;
        bbSolver1.setBoundType(1);
        start = chrono::high_resolution_clock::now();
        bbSolver1.bb4TSP(matrix, n);
        end = chrono::high_resolution_clock::now();
        auto bbTime1 = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "   Time: " << bbTime1 << " ms" << endl;
        cout << "   Best Cost: " << bbSolver1.getMinCost() << endl;
        cout << "   Nodes Explored: " << bbSolver1.getNodeCount() << endl;

        // Test Branch-and-Bound - Bound Function 2
        cout << "\n[3] Branch-and-Bound (Bound Function 2 - Min In/Out Edges):" << endl;
        BB4TSP bbSolver2;
        bbSolver2.setBoundType(2);
        start = chrono::high_resolution_clock::now();
        bbSolver2.bb4TSP(matrix, n);
        end = chrono::high_resolution_clock::now();
        auto bbTime2 = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "   Time: " << bbTime2 << " ms" << endl;
        cout << "   Best Cost: " << bbSolver2.getMinCost() << endl;
        cout << "   Nodes Explored: " << bbSolver2.getNodeCount() << endl;

        // Test Branch-and-Bound - Bound Function 3
        cout << "\n[4] Branch-and-Bound (Bound Function 3 - Improved Bound):" << endl;
        BB4TSP bbSolver3;
        bbSolver3.setBoundType(3);
        start = chrono::high_resolution_clock::now();
        bbSolver3.bb4TSP(matrix, n);
        end = chrono::high_resolution_clock::now();
        auto bbTime3 = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "   Time: " << bbTime3 << " ms" << endl;
        cout << "   Best Cost: " << bbSolver3.getMinCost() << endl;
        cout << "   Nodes Explored: " << bbSolver3.getNodeCount() << endl;

        // Performance Comparison Summary
        cout << "\n----------------------------------------" << endl;
        cout << "Performance Summary" << endl;
        cout << "----------------------------------------" << endl;

        cout << ">> Backtracking vs Branch-and-Bound 1:" << endl;
        if (bbTime1 > 0) {
            cout << "   Speedup: " << fixed << setprecision(2)
                 << (double)backTime / bbTime1 << "x" << endl;
            cout << "   Node Reduction: " << fixed << setprecision(2)
                 << (1.0 - (double)bbSolver1.getNodeCount() / backSolver.getNodeCount()) * 100 << "%" << endl;
        } else {
            cout << "   Branch-and-Bound 1 is extremely fast (< 1ms)" << endl;
        }

        cout << "\n>> Backtracking vs Branch-and-Bound 2:" << endl;
        if (bbTime2 > 0) {
            cout << "   Speedup: " << fixed << setprecision(2)
                 << (double)backTime / bbTime2 << "x" << endl;
            cout << "   Node Reduction: " << fixed << setprecision(2)
                 << (1.0 - (double)bbSolver2.getNodeCount() / backSolver.getNodeCount()) * 100 << "%" << endl;
        } else {
            cout << "   Branch-and-Bound 2 is extremely fast (< 1ms)" << endl;
        }

        cout << "\n>> Backtracking vs Branch-and-Bound 3:" << endl;
        if (bbTime3 > 0) {
            cout << "   Speedup: " << fixed << setprecision(2)
                 << (double)backTime / bbTime3 << "x" << endl;
            cout << "   Node Reduction: " << fixed << setprecision(2)
                 << (1.0 - (double)bbSolver3.getNodeCount() / backSolver.getNodeCount()) * 100 << "%" << endl;
        } else {
            cout << "   Branch-and-Bound 3 is extremely fast (< 1ms)" << endl;
        }

        cout << "\n>> Branch-and-Bound Bound Function Comparison:" << endl;
        cout << "   Bound Function 1: " << bbTime1 << " ms, "
             << bbSolver1.getNodeCount() << " nodes" << endl;
        cout << "   Bound Function 2: " << bbTime2 << " ms, "
             << bbSolver2.getNodeCount() << " nodes" << endl;
        cout << "   Bound Function 3: " << bbTime3 << " ms, "
             << bbSolver3.getNodeCount() << " nodes" << endl;

        // Find the best Branch-and-Bound method
        long long minBBTime = min({bbTime1, bbTime2, bbTime3});
        string bestBB = "";
        if (minBBTime == bbTime1) bestBB = "Bound Function 1";
        else if (minBBTime == bbTime2) bestBB = "Bound Function 2";
        else bestBB = "Bound Function 3";

        cout << "   Best Bound Function: " << bestBB
             << " (" << minBBTime << " ms)" << endl;

        cout << "\n----------------------------------------" << endl;
        cout << endl << endl;
    }

    cout << "========================================" << endl;
    cout << "All Tests Completed!" << endl;
    cout << "========================================" << endl;

    return 0;
}