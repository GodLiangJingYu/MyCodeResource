/**
 * Lengest common sequence problem
 *
 * @author luwei
 * @version 1.1 
 * 2017-11-7
 */

public final class LCS {

	/**
	 * 最长公共子序列求解算法
	 * @param s1 第一个序列，长度为m
	 * @param s2 第二个序列，长度为n
	 * @param t 最长公共子序列追踪矩阵，m+1行,n+1列
	 * @return 最长公共子序列长度
	 */
	public static int longestComSeq(String s1, String s2, int[][] t){
		char[] seq1 = s1.toCharArray();
		char[] seq2 = s2.toCharArray();

		int[][] res = new int[seq1.length+1][seq2.length+1];
		
		return longestComSeq(seq1, seq2, res, t);
	}
	
	/**
	* 最长公共子序列求解算法
	* 
	* @param s1 第一个序列，长度为m
	* @param s2 第二个序列，长度为n
	* @param res 最长公共子序列递推求解结果矩阵，m+1行,n+1列，两个长度为0的序列最大匹配长度为0，为递推关系初值
	* @param tra 最长公共子序列追踪矩阵，m+1行,n+1列
	* @return 最长公共子序列长度
	*/
	public static int longestComSeq(char[] s1, char[] s2, int[][] res, int[][] tra) {

		tra[0][0] = 0; 
		
		for(int i=0; i<s1.length+1; i++)	res[i][0] = 0;//结果矩阵第0列赋初值0
		for(int j=1; j<s2.length+1; j++)	res[0][j] = 0;//结果矩阵第0行赋初值0

		for(int i=1; i<s1.length+1; i++){
			for(int j=1; j<s2.length+1; j++){
				if(s1[i-1] == s2[j-1]){//尾部字符匹配
					res[i][j] = res[i-1][j-1] + 1;
					tra[i][j] = 1;//表示继续向左上tra[i-1][j-1]追踪
				}
				else if(res[i-1][j] >= res[i][j-1]){//当前最长公共子序列由s1尾部前移一位与s2构成
					res[i][j] = res[i-1][j];
					tra[i][j] = 2;//2表示继续向上tra[i-1][j]追踪
				}
				else{//当前最长序列由s2尾部前移一位与s1构成
					res[i][j] = res[i][j-1];
					tra[i][j] = 3;//3表示继续向左tra[i][j-1]追踪
				}
			}
		}
		return res[s1.length][s2.length];
	}

	/**
	* 最长公共子序列追踪算法
	* 
	* @param s1 第一个序列
	* @param tra 最长公共子序列追踪矩阵，m+1行,n+1列
	* @param tailS1 第一个序列的结束位置，1,2,...,m
	* @param tailS2 第二个序列的结束位置，1,2,...,n
	*/
	public static void traceback(char[] s1, int[][] tra, int tailS1, int tailS2){
		if  (tailS1 == 0 || tailS2 == 0) return;

		if  (tra[tailS1][tailS2] == 1) {//向左上追踪tra[i-1][j-1]
			traceback(s1, tra, tailS1-1, tailS2-1);
			System.out.print(s1[tailS1-1] + " ");
		}
		else if (tra[tailS1][tailS2] == 2) {//继续向上追踪tra[i-1][j]
			traceback(s1, tra, tailS1-1, tailS2);
		}
		else traceback(s1, tra, tailS1, tailS2-1);//继续向左tra[i][j-1]追踪
	}

	public static void main(String[] args){

		char[] s1 = {'a', 'b', 'c', 'b', 'd', 'a', 'b'};
		char[] s2 = {'b', 'd', 'c', 'a', 'b', 'a'};
//		String seq1 = "abcbdab";
//		String seq2 = "bdcaba";
		
//		char[] s1 = {'1', '2', '3', '4', '5', '6', '7', '8'};
//		char[] s2 = {'1', '4', '2', '3', '6', '8', '5', '7'};		

		System.out.print("sequence1: ");
		for(int i=0; i<s1.length; i++){
			System.out.print(s1[i] + " ");
		}
		System.out.println();
		System.out.print("sequence2: ");
		for(int i=0; i<s2.length; i++){
			System.out.print(s2[i] + " ");
		}
		System.out.println();

		int[][] resultMatrix = new int[s1.length+1][s2.length+1];
		int[][] traceMatrix = new int[s1.length+1][s2.length+1];
		
		System.out.println("The length of the longest common sequence is: " + longestComSeq(s1, s2, resultMatrix, traceMatrix));

//		System.out.println("The length of the longest common sequence is: " + longestComSeq(seq1, seq2, traceMatrix));

		System.out.print("The longest common sequece is: ");
		traceback(s1, traceMatrix, s1.length, s2.length);
	}
}