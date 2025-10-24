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
			//TODO
		} else {
			//TODO

		}

	}

	private void swap(int i, int j) {
		int temp = x[i];
		x[i] = x[j];
		x[j] = temp;
	}

	public boolean check(int pos) {
		//TODO
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
