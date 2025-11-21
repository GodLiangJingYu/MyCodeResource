//
// Created by 86180 on 2025/11/20.
//
AircraftParking(m, n, X, Y, C):
   3     bestCost ← ∞
   4     bestSolution ← null
   5     currentSolution[1..m] ← 0
   6     used[1..n] ← false  // 标记停机位是否被使用
   7
   8     // 回溯函数
   9     function Backtrack(depth, currentCost):
  10         if depth = m + 1 then:  // 所有飞机都已分配
  11             if currentCost < bestCost then:
  12                 bestCost ← currentCost
  13                 bestSolution ← copy(currentSolution)
  14             return
  15
  16         // 剪枝：当前费用已超过最优解
  17         if currentCost ≥ bestCost then:
  18             return
  19
  20         for j ← 1 to n do:
  21             if not used[j] and X[depth] ≤ Y[j] then:
  22                 // 选择停机位j
  23                 currentSolution[depth] ← j
  24                 used[j] ← true
  25                 newCost ← currentCost + C[depth][j]
  26
  27                 // 递归到下一架飞机
  28                 Backtrack(depth + 1, newCost)
  29
  30                 // 回溯
  31                 used[j] ← false
  32                 currentSolution[depth] ← 0
  33
  34     Backtrack(1, 0)
  35     return bestSolution, bestCost