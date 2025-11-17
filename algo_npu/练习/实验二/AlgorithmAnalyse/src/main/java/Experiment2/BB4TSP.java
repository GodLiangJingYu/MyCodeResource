package Experiment2;
import java.util.Collections;
import java.util.LinkedList;
import java.util.Vector;

public class BB4TSP {

	int NoEdge = -1; //��ʾû�б�
	private int minCost = Integer.MAX_VALUE; //��ǰ��С����
	public int getMinCost() {
		return minCost;
	}

	public void setMinCost(int minCost) {
		this.minCost = minCost;
	}

	private LinkedList<HeapNode> heap = new LinkedList<HeapNode>();//�洢��ڵ�
	private Vector<Integer> bestH = new Vector<Integer>();
	
	
	@SuppressWarnings("rawtypes")
	public static class HeapNode implements Comparable{
		Vector<Integer> liveNode;//��������
		int lcost; //���۵��½�
		int level;//0-level�ĳ������Ѿ��źõ�
		//���췽��
		public HeapNode(Vector<Integer> node,int lb, int lev){
			liveNode.addAll(0, node);
			lcost = lb;
			level = lev;
		}
		
		@Override
		public int compareTo(Object x) {//��������, ÿһ��pollFirst
			int xu=((HeapNode)x).lcost;
			if(lcost<xu) return -1;
			if(lcost==xu) return 0;
			return 1;
		}
		public boolean equals(Object x){
			return lcost==((HeapNode)x).lcost;
		}

	}
	
	/**
	 * ���㲿�ֽ���½�.
	 * 
	 * @param liveNode 
	 * 		              ���е���й
	 *  
	 * @param n   
	 * 			   ��ǰȷ���ĳ��еĸ���.
	 * @param cMatrix
	 *            �ڽӾ��󣬵�0�У�0�в���
	 * 
	 * @exception IllegalArgumentException
	 */
	public int computeLB(Vector<Integer> liveNode, int level, int[][] cMatrix)
	{
		//TODO: 计算当前部分路径的下界（Lower Bound）
		// 提示：使用最小生成树或简化方法计算下界，用于剪枝
		// 步骤：1. 计算已确定路径的花费 2. 估算剩余路径的最小可能花费 3. 返回总和作为下界
		// 注意：下界应该小于等于实际最优解，确保不会剪掉最优解
		return -1;
	}
	
	/**
	 * ����TSP�������С���۵�·��.
	 * 
	 * @param cMatrix
	 *            �ڽӾ��󣬵�0�У�0�в���
	 * @param n   ���и���.
	 * @exception IllegalArgumentException
	 */
	public int bb4TSP(int[][] cMatrix, int n)
	{
		//�����ʼ�ڵ�
		Vector<Integer> liveNode = new Vector<Integer>() ;//��������
		for(int i = 1; i<=n; i++) liveNode.add(i);
		int level = 1;//0-level�ĳ������Ѿ��źõ�
		int lcost = computeLB(liveNode, level, cMatrix) ; //���۵��½�
		while(level != n)
		{
			//TODO: 分支限界法主循环
			// 提示：使用优先队列存储活节点，每次选择下界最小的节点扩展
			// 步骤：1. 从当前节点生成子节点 2. 计算每个子节点的下界 3. 加入优先队列 4. 选择最优节点继续扩展
			//参考优先队列，不断扩展节点，选取下一个节点
		}
		
		return minCost;
	}
	
	
}
