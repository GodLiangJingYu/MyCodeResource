/**
 * 电路单层最大布线问题
 *
 * @author luwei
 * @version 1.0 2017-11-15
 */

public final class MNS{

	/**
	 * 单层最大布线数求解算法
	 * @param perm 顺序接线柱连接点的一个排列
	 * @param size 最大布线数目存储方阵，阶为perm长度+1
	 * @return 单层最大布线数目
	 */
	public static int maxNetSet(int[] perm, int[][] size){
		int portNum = perm.length;

		//矩阵赋初值，上端0个接线柱情况(i=0)，最大布线数为0
		for(int j=0; j<=portNum; j++)	size[0][j] = 0;

		for(int i=1; i<=portNum; i++){//上端接线柱序列
			for(int j=0; j<perm[i-1]; j++){//下端接线柱序列
				size[i][j] = size[i-1][j];//第i号导线无法放
			}
			for(int j=perm[i-1]; j<=portNum; j++){//下端接线柱序列
				size[i][j] = max(size[i-1][perm[i-1]-1]+1, size[i-1][j]);//取放第i号导线和不放第i号导线中的最大布线数值
			}
		}
		return size[portNum][portNum];
	}

	/**
	 * 追踪最大可放置导线编号
	 * @param perm 导线连接排列标号
	 * @param size 最大可放置导线数量追踪矩阵
	 * @param lines 返回参数，存储最大可放置导线编号
	 */
	public static void traceback(int[] perm, int[][] size, int[] lines){

		int n = perm.length;
		int j = n;
		int linesNum = 0;//放置导线数量计数器

		for(int i=n; i>0; i--){
			if(size[i][j] != size[i-1][j]){//放第i号导线
				lines[linesNum++] = i;
				j = perm[i-1] - 1;//放进第i号导线之后变更j下标为perm[i-1],perm[i-1]对应第i号导线连接位置
			}
		}
	}

	public static void outputLines(int[] perm, int[][] size, int linesNum){
		
		int n = perm.length;
		int[] lines = new int[linesNum];

		traceback(perm, size, lines);

		System.out.print("Lines No: " + lines[linesNum-1]);
		for(int i=linesNum-2; i>=0; i--){//输出导线编号，出栈顺序
			System.out.print(", " + lines[i]);
		}
		System.out.println();
	}

	private static int max(int a, int b){
		return a > b ? a : b;
	}

	public static void main(String[] args){
		
		int[] perm = {1, 4, 2, 3, 6, 8, 5, 7};
		int n = perm.length;
		int[][] size = new int[n+1][n+1];

		int[] lines = new int[n];
		int[] linesNum = new int[1];

		System.out.print("上接线柱标号: " + perm[0]);
		for(int i=1; i<n; i++)
			System.out.print(", " + perm[i]);
		System.out.println();

		System.out.print("下接线柱标号: " + 1);
		for(int i=2; i<=n; i++)
			System.out.print(", " + i);
		System.out.println();

		int maxLinesNum = maxNetSet(perm, size);
		System.out.println(maxLinesNum);

		outputLines(perm, size, maxLinesNum);
	}
}