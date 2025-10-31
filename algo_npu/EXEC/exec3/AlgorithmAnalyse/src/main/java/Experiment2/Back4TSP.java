package Experiment2;

public class Back4TSP {

    int NoEdge = -1;                 // 表示两城市之间没有直接边
    int bigInt = Integer.MAX_VALUE;  // 极大值，用作初始最优解
    int[][] a;                       // 邻接矩阵（距离矩阵）
    int cc = 0;                      // 当前已走路径长度
    int bestc = bigInt;              // 当前最优路径长度
    int[] x;                         // 当前解（城市访问顺序，x[1] 固定为 1）
    int[] bestx;                     // 当前最优解
    int n = 0;                       // 城市数量

    /**
     * 回溯主函数
     * @param i 当前要确定的位置（从 2 开始，因为 x[1]=1 已固定）
     */
    private void backtrack(int i) {
        if (i > n) {                                 // 已经排好 1..n，得到一个完整回路
            // 加上从最后一个城市回到起点 1 的距离
            if (a[x[n]][x[1]] != NoEdge && cc + a[x[n]][x[1]] < bestc) {
                bestc = cc + a[x[n]][x[1]];
                System.arraycopy(x, 1, bestx, 1, n);  // 记录最优解（只复制 1..n）
            }
        } else {
            for (int j = i; j <= n; j++) {           // 尝试把位置 i 与 i..n 中的每个城市交换
                // 剪枝：如果当前边不存在或已不优于 bestc，则直接跳过
                if (!check(i, j)) continue;

                swap(i, j);                          // 交换产生下一个排列
                int prev_cc = cc;                    // 保存进入递归前的累计长度
                cc += a[x[i - 1]][x[i]];             // 累加新边
                backtrack(i + 1);                    // 递归下一层
                cc = prev_cc;                        // 恢复现场
                swap(i, j);                          // 恢复交换
            }
        }
    }

    /**
     * 交换数组中两个位置的值
     */
    private void swap(int i, int j) {
        int temp = x[i];
        x[i] = x[j];
        x[j] = temp;
    }

    /**
     * 剪枝判断
     * @param pos 当前要填的位置（i）
     * @param j   将要与 pos 交换的候选位置
     * @return true 表示可以继续尝试该分支
     */
    private boolean check(int pos, int j) {
        // 1. 边必须存在
        if (a[x[pos - 1]][x[j]] == NoEdge) return false;

        // 2. 当前累计 + 剩余最小估计 >= bestc，直接剪枝
        // 为了简单，这里采用一个保守的下界：
        //    剩余城市（包括当前要放的）至少要走 (n-pos+1) 条边，每条边取矩阵中的最小正值
        int minEdge = bigInt;
        for (int p = 1; p <= n; p++) {
            for (int q = 1; q <= n; q++) {
                if (a[p][q] != NoEdge && a[p][q] < minEdge) {
                    minEdge = a[p][q];
                }
            }
        }
        if (minEdge == bigInt) minEdge = 0;   // 没有边的情况（理论上不会出现）

        int remain = n - pos + 1;              // 还剩多少条边（包括当前要加的）
        if (cc + a[x[pos - 1]][x[j]] + remain * minEdge >= bestc) {
            return false;
        }
        return true;
    }

    /**
     * 公共接口：求解 TSP
     * @param b   距离矩阵（下标从 1 开始，b[i][j] 表示 i→j 的距离，-1 表示无边）
     * @param num 城市数量
     */
    public void backtrack4TSP(int[][] b, int num) {
        n = num;
        x = new int[n + 1];
        bestx = new int[n + 1];
        a = b;

        // 初始化：起点固定为城市 1
        for (int i = 1; i <= n; i++) x[i] = i;
        bestc = bigInt;
        cc = 0;

        backtrack(2);   // 从第 2 个位置开始回溯

        // 输出结果（这里直接打印，实际使用时可自行封装返回）
        if (bestc == bigInt) {
            System.out.println("无解");
        } else {
            System.out.println("最短路径长度 = " + bestc);
            System.out.print("路径：");
            for (int i = 1; i <= n; i++) {
                System.out.print(bestx[i] + (i < n ? " -> " : " -> 1\n"));
            }
        }
    }

    /* ------------------- 测试代码 ------------------- */
    public static void main(String[] args) {
        // 示例：4 个城市，距离矩阵（-1 表示无边）
        int[][] graph = {
                //  1   2   3   4
                {0,  0,  0,  0},   // 0 号下标占位
                {0,  0, 10, 15, 20},
                {0, 10,  0, 35, 25},
                {0, 15, 35,  0, 30},
                {0, 20, 25, 30,  0}
        };

        Back4TSP solver = new Back4TSP();
        solver.backtrack4TSP(graph, 4);
    }
}