/**
* This class use for tesing intrger partition algorithm
*
* @author luwei
* @version 1.1
* 2017-10-29
*/

public final class IntegerPartition
{
	public static int partition(int n, int m){

		if(n<1||m<1) return 0;
		
		if(m==1||n==1) return 1;
		
		if(m>n) return partition(n,n);
		
		if(m==n) return partition(n,m-1)+1;
		
		return partition(n,m-1)+partition(n-m,m);
	}

	public static void main(String[] args){
		int n=2;
		int m=2;
		System.out.println(partition(n,m));
	}
}