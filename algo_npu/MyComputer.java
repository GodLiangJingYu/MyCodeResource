/**
* This class use for common computing 
*
* @author luwei
* @version 1.0
* @date 2017-10-29
*/

public final class MyComputer{

	/**
	 * @param n>=0
	 * @return n!
	 */
	public static long factorialRec(int n){
		if(n==0) return 1;
		return n*factorialRec(n-1);
	}
	
	/**
	 * @param n>=0
	 * @return n!
	 */
	public static long factorialNRec(int n){
		if(n <= 1) return 1;
		long fn=1;
		for(int i=2; i<=n; i++)
			fn = fn*i;
		return fn;
	}

	/**
	 * @param n>=0
	 * @return fibonacci value of n
	 */
	public static int fibonacciRec(int n){
		if(n<=1) return n;
		return fibonacciRec(n-1)+fibonacciRec(n-2);
	}

	/**
	 * @param n>=0
	 * @return fibonacci value of n
	 */
	public static int fibonacciIter(int n){
		int[] a = new int[2];
		a[0]=0; a[1]=1;

		for(int i=2; i<=n; i++){
			a[i%2] = a[(i-1)%2] + a[(i-2)%2];
		}
		return a[n%2];
	}

	/**
	 * @param n>=0
	 * @return a^n
	 */
	public static int pow(int a, int n){
		if(n == 0) return 1;
		if(n == 1) return a;

		if(n%2 == 0){
			int b = pow(a, n/2);
			return b*b;
		}
		else{
			int b = pow(a, (n-1)/2);
			return a*b*b;
		}
	}

	/**
	 * ¾ØÕó³Ë·¨
	 * aºÍbÎª·Ç¿Õ¿É³Ë¾ØÕó
	 */
	public static int[][] matrixMultiply(int[][] a, int[][] b){
		int ra = a.length, ca = a[0].length;
		int rb = b.length, cb = b[0].length;
/*		
		if(ca != rb){
			error("¾ØÕó²»¿É³Ë£¡");
			return null;
		}
*/
		int[][] c = new int[ra][cb];
		for(int i=0; i<ra; i++){
			for(int j=0; j<cb; j++){
				int sum = 0;
				for(int k=0; k<ca; k++){
					sum += a[i][k]*b[k][j];
				}
				c[i][j] = sum;
			}
		}
		return c;
	}
/*
	private static void error(String err){
		System.out.println(err);
	}
*/	 

	public static void main(String[] args){	
		
		long begin1 = System.currentTimeMillis();
		System.out.println(factorialRec(20));
		long end1 = System.currentTimeMillis();
		System.out.println("Recursive computing time: " + (end1 - begin1) + "ºÁÃë");

		long begin2 = System.currentTimeMillis();
		System.out.println(factorialNRec(20));
		long end2 = System.currentTimeMillis();
		System.out.println("Iterative computing time: " + (end2 - begin2) + "ºÁÃë");

		long begin3 = System.currentTimeMillis();
		System.out.println(fibonacciRec(45));
		long end3 = System.currentTimeMillis();
		System.out.println("Recursive computing time: " + (end3 - begin3) + "ºÁÃë");

		long begin4 = System.currentTimeMillis();
		System.out.println(fibonacciIter(45));
		long end4 = System.currentTimeMillis();
		System.out.println("Iterative computing time: " + (end4 - begin4) + "ºÁÃë");

//		System.out.println(pow(2, 6));
	}

	/**
	 * µ÷ÊÔ¾ØÕó³Ë·¨
	 *
	 */
	/*
	public static void main(String[] args){
		int[][] a = {{1,1,1},{1,1,1}};
		int[][] b = {{1,1},{1,1},{1,1}};
		int ra = a.length, ca = a[0].length;
		int rb = b.length, cb = b[0].length;

		for(int i=0; i<ra; i++){
			for(int j=0; j<ca; j++)
				System.out.print(a[i][j] + " ");
			System.out.println();
		}
		for(int i=0; i<2*ca; i++)
			System.out.print("=");
		System.out.println();

		for(int i=0; i<rb; i++){
			for(int j=0; j<cb; j++)
				System.out.print(b[i][j] + " ");
			System.out.println();
		}
		for(int i=0; i<2*ca; i++)
			System.out.print("=");
		System.out.println();

		int[][] c = matrixMultiply(a, b);

		if(c != null){
			for(int i=0; i< c.length; i++){
				for(int j=0; j<c[0].length; j++){
					System.out.print(c[i][j] + " ");
				}
				System.out.println();
			}
		}
	}
	*/
}