/**
* Matrix chain problem
*
* @author luwei
* @version 1.0
* @date 2017-11-5
*/

import java.util.*;

public final class MatrixChain{
	
	/**
	 * 求解矩阵链最小乘次
	 * @param p n个矩阵的n+1个行列值,p[0,...,n]
	 * @param t 分裂点追踪矩阵,n+1阶,t[0,...,n]
	 * @return 矩阵链p的最小乘次
	 */
	public static int minMatrixChain(int[] p, int[][] t){

		return recurMatrixChain(p, t, 1, p.length-1);

//		return matrixChain(p, t);
	}

	/**
	 * 递归方法求解矩阵连乘问题最优解
	 * @param p n个矩阵的n+1个行列值,p[0,...,n]
	 * @param t 分裂点追踪矩阵,n+1阶,t[0,...,n]
	 * @param i 矩阵链的起始位置,i>0
	 * @param j 矩阵链的结束位置,j<=n
	 * @return 矩阵链p的最小乘次
	 */
	public static int recurMatrixChain(int[] p, int[][] t, int i, int j) {
		if(i == j) return 0;
		
		int minValue = recurMatrixChain(p, t, i, i) + recurMatrixChain(p, t, i+1, j) + p[i-1]*p[i]*p[j];//矩阵链从i后位置断开
		t[i][j] = i;
		for(int k=i+1; k<j; k++){//在i+1后的位置到j-1后的位置遍历断开，求解最小乘次
			int temp = recurMatrixChain(p, t, i, k) + recurMatrixChain(p, t, k+1, j) + p[i-1]*p[k]*p[j];
			if(temp < minValue){
				minValue = temp;
				t[i][j] = k;
			}
		}
		return minValue;
	}

	/**
	 * 非递归方法求解矩阵连乘问题最优解
	 * @param p n个矩阵的n+1个行列值,p[0,...,n]
	 * @param t 分裂点追踪矩阵,n+1阶,t[0,...,n]
	 * @return 矩阵链p的最小乘次
	 */
	public static int matrixChain(int[] p, int[][] t){

		int[][] multiplyNumMatrix = new int[p.length][p.length];//最小乘次矩阵
		for(int i=0; i<p.length; i++){
			for(int j=0; j<p.length; j++){
				multiplyNumMatrix[i][j] = -1;
				t[i][j] = -1;//追踪矩阵
			}
		}

		for(int i=0; i<multiplyNumMatrix.length; i++) multiplyNumMatrix[i][i] = 0;//一个矩阵乘次为0，即步长为1的矩阵链乘次为0

		for(int r=2; r<p.length; r++){//步长从2到矩阵链长度
			for(int i=1; i<=p.length-r; i++){//计算所有步长为r的链中的最小乘次链
				int j = i + r - 1;
				multiplyNumMatrix[i][j] = multiplyNumMatrix[i+1][j] + p[i-1]*p[i]*p[j];
				t[i][j] = i;
				for(int k=i+1; k<j; k++){
					int temp = multiplyNumMatrix[i][k] + multiplyNumMatrix[k+1][j] + p[i-1]*p[k]*p[j];
					if(temp < multiplyNumMatrix[i][j]){
						multiplyNumMatrix[i][j] = temp;
						t[i][j] = k;
					}
				}
			}
		}
		return multiplyNumMatrix[1][p.length-1];
	}

	public static void traceback(int i, int j, int[][] t){
		if(i == j)	return;

		traceback(i, t[i][j], t);
		traceback(t[i][j]+1, j, t);

		System.out.print(i + "," + t[i][j] + "and");
		System.out.print(t[i][j]+1);
		System.out.print("," + j);
		System.out.println();
	}

	public static void main(String[] args) {
		
		int[] matrixChain = {30, 35, 15, 5, 10, 20, 25};
		int matrixLength = matrixChain.length - 1;
		int[][] traceMatrix = new int[matrixLength+1][matrixLength+1];

		System.out.println("Matrix: ");
		System.out.print(matrixChain[0] + "×" + matrixChain[1]);
		for(int i=2; i<=matrixLength; i++){
			System.out.print("," + matrixChain[i-1] + "×" + matrixChain[i]);
		}
		System.out.println();

//		int minMultiplyNum = recurMatrixChain(matrixChain, traceMatrix, 1, matrixLength);
//		int minMultiplyNum = matrixChain(matrixChain, traceMatrix);
		int minMultiplyNum = minMatrixChain(matrixChain, traceMatrix);
		System.out.println("The min multiply num: " + minMultiplyNum);

		traceback(1, matrixLength, traceMatrix);
	}
}
