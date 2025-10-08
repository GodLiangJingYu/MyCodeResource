/**
 * 最大子段和问题
 *
 * @author luwei
 * @version 1.0 2017-11-19
 */

public final class MaxSumSeg{

	public static int maxSum(int[] seq, int[] bestSeg){
		
//		return maxSumBruteForce1(seq, bestSeg);
		
//		return maxSumBruteForce2(seq, bestSeg);

//		return maxSumRec(seq, 0, seq.length-1);

		return maxSumProg(seq, bestSeg);
	}
	/**
	 * 蛮力法求解整数序列最大子段和O(n^3)
	 * @param seq 整数序列
	 * @param bestSeg 返回参数，存储最大和段的起始和结束下标
	 * @return 最大子段和
	 */
	public static int maxSumBruteForce1(int[] seq, int[] bestSeg){

		int len = seq.length;
		int sum = Integer.MIN_VALUE;

		for(int i=0; i<len; i++){//子段起始位置
			for(int j=i; j<len; j++){//子段结束位置
				int thissum = 0;
				for(int k=i; k<=j; k++){//子段求和
					thissum += seq[k];
				}
				if(thissum > sum){//当前子段和最大
					sum = thissum;
					bestSeg[0] = i;
					bestSeg[1] = j;
				}
			}
		}
		return sum;
	}

	/**
	 * Same as 蛮力法 O(n^2)
	 */
	public static int maxSumBruteForce2(int[] seq, int[] bestSeg){

		int len = seq.length;
		int sum = Integer.MIN_VALUE;

		for(int i=0; i<len; i++){//子段起始位置
			int thissum = 0;
			for(int j=i; j<len; j++){//子段结束位置				
				thissum += seq[j];				
				if(thissum > sum){//当前子段和最大
					sum = thissum;
					bestSeg[0] = i;
					bestSeg[1] = j;
				}
			}
		}
		return sum;
	}

	/**
	 * 分治法求解最大子段和O(nlogn)
	 * @param seq 整数序列
	 * @param left 序列起始位置
	 * @param right 序列结束位置
	 * @param 
	 * @return 最大子段和
	 */
	public static int maxSumRec(int[] seq, int left, int right){

		int sum;

		if(left == right){
			sum = seq[left];
		}
		else{

			int center = (left+right)/2;

			int leftSum = maxSumRec(seq, left, center);
			int rightSum = maxSumRec(seq, center+1, right);

			//由center向左最大子段和
			int maxSumL = Integer.MIN_VALUE, lefts = 0;

			for(int i=center; i>=left; i--){
				lefts += seq[i];
				if(lefts > maxSumL){
					maxSumL = lefts;
				}
			}

			//由center向右最大子段和
			int maxSumR = Integer.MIN_VALUE, rights = 0;

			for(int i=center+1; i<=right; i++){
				rights += seq[i];
				if(rights > maxSumR){
					maxSumR = rights;
				}
			}

			sum = maxSumL + maxSumR;
		
			if((leftSum > sum) && (leftSum >= rightSum)){//取左段
				sum = leftSum;
			}
			if((rightSum > sum) && (rightSum > leftSum)){//取右段
				sum = rightSum;
			}
		}
		return sum;
	}

	/**
	 * 动态规划算法求解最大子段和O(n)
	 * @param seq 整数序列
	 * @param bestSeg 返回参数，存储最大和段的起始和结束下标
	 * @return 最大子段和
	 */
	public static int maxSumProg(int[] seq, int[] bestSeg){
		
		int n = seq.length;
		int sum = Integer.MIN_VALUE, partSum = 0;
		int bestiMaybe = 0;//用于标记每次重新开始求和的初始位置下标
	
		bestSeg[0] = 0;	
	
		for(int i=0; i<n; i++){		
			if(partSum > 0) {
				partSum += seq[i];
			}
			else {
				partSum = seq[i];
				bestiMaybe = i;  //标记当前初始下标
			}		
			if(partSum > sum) {
				sum = partSum;
				bestSeg[0] = bestiMaybe; //更新初始下标
				bestSeg[1] = i;  //更新最大段结束下标
			}
		}
		return sum;
	}

	/**
	 * 2018年博士入学试题
	 * 已知A是由不相同整数构成的序列，设计算法求A中最长的单调递增子段。
	 * 最长的单调递增子序列
	 */
	public static int longestASCSeq(int[] seq, int[]){
		return 0;
	}

	public static void main(String[] args){
		
//		int[] sequence = {-2, 11, -4, 13, -5, -2};
		int[] sequence = {2, -5, 8, 11, -3, 4, 6};
//		int[] sequence = {-1, 1};

		int[] bestSeg = {0, 0};

		int seqLen = sequence.length;

		System.out.print("The integer sequence: " + sequence[0]);
		for(int i=1; i<seqLen; i++)
			System.out.print(", " + sequence[i]);
		System.out.println();

		System.out.println("The max sum of segment in the sequence: " + maxSum(sequence, bestSeg));
		System.out.println("The best segment: " + "from sequence[" + bestSeg[0] + "] to sequence[" + bestSeg[1] + "]");
	}
}