package Experiment2;

import java.util.Collections;
import java.util.LinkedList;
import java.util.Vector;

public class BB4TSP {

    int NoEdge = -1; // 表示无边
    private int minCost = Integer.MAX_VALUE; // 当前最小耗费
    public int getMinCost() { return minCost; }
    public void setMinCost(int minCost) { this.minCost = minCost; }

    private LinkedList<HeapNode> heap = new LinkedList<>(); // 最小堆（活节点表）
    private Vector<Integer> bestH = new Vector<>();         // 最优路径

    @SuppressWarnings("rawtypes")
    public static class HeapNode implements Comparable {
        Vector<Integer> liveNode; // 剩余可选城市集合
        int lcost;                // 当前路径下界
        int level;                // 已确定城市数（从1开始）
        int ccost;                // 当前实际路径耗费（到 level 为止）

        public HeapNode(Vector<Integer> node, int lb, int lev, int cost) {
            this.liveNode = new Vector<>(node);
            this.lcost = lb;
            this.level = lev;
            this.ccost = cost;
        }

        @Override
        public int compareTo(Object x) {
            int xu = ((HeapNode) x).lcost;
            if (lcost < xu) return -1;
            if (lcost == xu) return 0;
            return 1;
        }

        public boolean equals(Object x) {
            return lcost == ((HeapNode) x).lcost;
        }
    }

    /**
     * 计算当前状态的下界（紧下界）
     * 方法：每行最小出边 + 每列最小入边（排除已用边）
     */
    public int computeLB(Vector<Integer> liveNode, int level, int[][] cMatrix) {
        int n = cMatrix.length - 1;
        int lb = 0;
        boolean[] rowUsed = new boolean[n + 1];   // 行是否已使用（出发城市）
        boolean[] colUsed = new boolean[n + 1];   // 列是否已使用（到达城市）

        // 标记已确定路径中的边
        for (int i = 1; i < level; i++) {
            int from = bestH.get(i - 1);
            int to = bestH.get(i);
            rowUsed[from] = true;
            colUsed[to] = true;
        }

        // 1. 每行（出发城市）取最小出边（未使用列）
        for (int i = 1; i <= n; i++) {
            if (rowUsed[i]) continue; // 已出发过
            int minOut = Integer.MAX_VALUE;
            for (int j = 1; j <= n; j++) {
                if (!colUsed[j] && cMatrix[i][j] != NoEdge) {
                    minOut = Math.min(minOut, cMatrix[i][j]);
                }
            }
            if (minOut == Integer.MAX_VALUE) return Integer.MAX_VALUE; // 无解
            lb += minOut;
        }

        // 2. 每列（到达城市）取最小入边（未使用行）
        for (int j = 1; j <= n; j++) {
            if (colUsed[j]) continue; // 已到达过
            int minIn = Integer.MAX_VALUE;
            for (int i = 1; i <= n; i++) {
                if (!rowUsed[i] && cMatrix[i][j] != NoEdge) {
                    minIn = Math.min(minIn, cMatrix[i][j]);
                }
            }
            if (minIn == Integer.MAX_VALUE) return Integer.MAX_VALUE;
            lb += minIn;
        }

        return lb;
    }

    /**
     * 分支限界求解 TSP
     */
    public int bb4TSP(int[][] cMatrix, int n) {
        // 清空状态
        heap.clear();
        bestH.clear();
        minCost = Integer.MAX_VALUE;

        // 起始节点：所有城市可选，level=1（将要选第一个城市）
        Vector<Integer> initNode = new Vector<>();
        for (int i = 1; i <= n; i++) initNode.add(i);

        // 固定起点为 1
        bestH.add(1); // 路径从 1 开始
        Vector<Integer> childNode = new Vector<>(initNode);
        childNode.remove((Integer)1);

        int initLB = computeLB(childNode, 1, cMatrix);
        heap.add(new HeapNode(childNode, initLB, 1, 0));

        while (!heap.isEmpty()) {
            HeapNode node = heap.pollFirst(); // 取出下界最小的活节点
            if (node.lcost >= minCost) continue; // 下界已不优

            int level = node.level;
            Vector<Integer> liveNode = node.liveNode;
            int ccost = node.ccost;

            if (level == n) {
                // 最后一个城市 → 回到起点 1
                int last = bestH.get(level - 1);
                if (cMatrix[last][1] != NoEdge) {
                    int total = ccost + cMatrix[last][1];
                    if (total < minCost) {
                        minCost = total;
                        bestH.add(1); // 闭合回路
                    }
                }
                continue;
            }

            // 扩展当前节点：尝试每个剩余城市作为下一个
            for (int i = 0; i < liveNode.size(); i++) {
                int nextCity = liveNode.get(i);
                int from = bestH.get(level - 1);
                if (cMatrix[from][nextCity] == NoEdge) continue;

                // 生成子节点
                Vector<Integer> newLive = new Vector<>(liveNode);
                newLive.remove(i);
                bestH.add(nextCity);
                int newCost = ccost + cMatrix[from][nextCity];
                int newLB = newCost + computeLB(newLive, level + 1, cMatrix);
                bestH.remove(bestH.size() - 1); // 回溯

                if (newLB < minCost) {
                    heap.add(new HeapNode(newLive, newLB, level + 1, newCost));
                }
            }

            // 维护最小堆：重新排序（LinkedList 模拟优先队列）
            Collections.sort(heap);
        }

        return minCost;
    }

    /* ------------------- 测试代码 ------------------- */
    public static void main(String[] args) {
        int[][] graph = {
                {0, 0, 0, 0, 0},
                {0, 0, 10, 15, 20},
                {0, 10, 0, 35, 25},
                {0, 15, 35, 0, 30},
                {0, 20, 25, 30, 0}
        };

        BB4TSP solver = new BB4TSP();
        int minCost = solver.bb4TSP(graph, 4);
        System.out.println("最小路径长度 = " + minCost);

        // 输出路径（bestH 已包含完整回路）
        System.out.print("路径：");
        for (int i = 0; i < solver.bestH.size(); i++) {
            System.out.print(solver.bestH.get(i));
            if (i < solver.bestH.size() - 1) System.out.print(" -> ");
        }
        System.out.println();
    }
}