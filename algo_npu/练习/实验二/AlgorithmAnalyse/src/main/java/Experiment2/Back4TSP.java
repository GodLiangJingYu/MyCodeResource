package Experiment2;

public class Back4TSP {

	int NoEdge = -1;
	int bigInt = Integer.MAX_VALUE;
	int[][] a; // �ڽӾ���
	int cc = 0; // �洢��ǰ����
	int bestc = bigInt;// ��ǰ���Ŵ���
	int[] x; // ��ǰ��
	int[] bestx;// ��ǰ���Ž�
	int n = 0; // �������
	
	private void backtrack(int i) {//iΪ��ʼ���
		if (i > n) {
			//TODO: 已经到达叶节点，需要计算当前路径的总花费
			// 提示：检查从最后一个城市回到起点的边是否存在，如果存在则更新最佳解
			// 步骤：1. 计算当前路径总花费 2. 与最佳解比较 3. 如果更优则更新bestc和bestx
		} else {
			//TODO: 递归搜索所有可能的城市排列
			// 提示：对于当前层i，尝试所有未访问的城市，递归调用backtrack(i+1)
			// 步骤：1. 循环遍历城市j从i到n 2. 交换x[i]和x[j] 3. 检查是否可行 4. 递归调用 5. 回溯(再次交换)
		}

	}
	
	private void swap(int i, int j) {
		int temp = x[i];
		x[i] = x[j];
		x[j] = temp;
	}
	
	public boolean check(int pos) {
		//TODO: 检查当前路径是否可行（剪枝函数）
		// 提示：检查从x[pos-1]到x[pos]的边是否存在，以及当前花费是否超过已知最优解
		// 步骤：1. 检查边是否存在 2. 计算当前路径花费 3. 与bestc比较
		// 返回值：如果路径可行且可能产生更优解则返回true，否则返回false
		return false;
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
