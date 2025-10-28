package Experiment2;

public class Back4TSP {

 int NoEdge = -1;
 int bigInt = Integer.MAX_VALUE;
 int[][] a; // 邻接矩阵
 int cc = 0; // 当前费用
 int bestc = bigInt;// 当前最优费用
 int[] x; // 当前解
 int[] bestx;// 当前最优解
 int n = 0; // 顶点数

 private void backtrack(int i) {// i为当前扩展位置
  if (i > n) {
   // 到达叶子，检查能否回到起点并更新最优
   if (a[x[n]][x[1]] != NoEdge) {
    int total = cc + a[x[n]][x[1]];
    if (total < bestc) {
     bestc = total;
     System.arraycopy(x, 1, bestx, 1, n);
    }
   }
  } else {
   // 试探第i位的城市
   for (int j = i; j <= n; j++) {
    // 前驱到候选的边必须存在
    if (a[x[i - 1]][x[j]] == NoEdge) continue;
    swap(i, j);
    cc += a[x[i - 1]][x[i]];
    if (check(i)) {
     backtrack(i + 1);
    }
    cc -= a[x[i - 1]][x[i]];
    swap(i, j);
   }
  }
 }

 private void swap(int i, int j) {
  int temp = x[i];
  x[i] = x[j];
  x[j] = temp;
 }

 public boolean check(int pos) {
  // 边存在性与剪枝：中间节点要求 cc < bestc；叶子还需能回到起点
  if (pos >= 2 && a[x[pos - 1]][x[pos]] == NoEdge) return false;
  if (pos == n) {
   if (a[x[n]][x[1]] == NoEdge) return false;
   return cc + a[x[n]][x[1]] < bestc;
  }
  return cc < bestc;
 }

 public void backtrack4TSP(int[][] b, int num) {
  n = num;
  x = new int[n + 1];
  for (int i = 0; i <= n; i++)
   x[i] = i;
  bestx = new int[n + 1];
  a = b;
  backtrack(2);
 }

}
