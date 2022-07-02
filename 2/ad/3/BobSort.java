import java.util.Random;
import java.lang.System.*;

public class BobSort {
  
  public static void swap(int[] a, int pos1, int pos2) {
    int tmp = a[pos1];
    a[pos1] = a[pos2];
    a[pos2] = tmp;
  }

  public static int smallerNumbersCount(int[] a, int v) {
    int result = 0;
    
    for (int i = 0; i < a.length; i++) {
      if (a[i] < v) {
        result++;
      }
    }
    
    return result;
  }
  
  public static void sort(int[] a) {

	for (int i = 0; i < a.length; i++) {
		System.out.print(i + " ");
	}
	System.out.println("");

    for (int i = 0; i < a.length;) {
      int newPos = smallerNumbersCount(a, a[i]);
      
      if ( newPos != i) {
		System.out.println("newPos : " + newPos);
        swap(a, i, newPos);
      } else {
        i++;
      }
    }
	for (int i = 0; i < a.length; i++) {
		System.out.print(i + " ");
	}
  }
  
  // immer dieselben Pseudozufallszahlen!
  private static final Integer DEFAULT_SEED = Integer.valueOf(654321);

  public static int[] createRandomArray(int n) {
    int[] a = new int[n];
    for (int i=0; i<n; ++i) 
      a[i] = i;
    // shuffle randomly.
    Random rand = new Random(DEFAULT_SEED*n); 
    for (int i=0; i<n; ++i) {
      int pos = rand.nextInt(n);
      swap(a, i, pos);
    }
    return a;
  }
  
  public static void runTest(int n) {

    long startTime;
    long endTime;

    int[] a = createRandomArray(n);
    startTime = System.currentTimeMillis();
    sort(a);
    endTime = System.currentTimeMillis();
    System.out.format(
        "[n=%d] : %d ms.\n",
        n, (endTime - startTime));
  }
  
  public static void main(String[] args) {
	int[] a = new int[] {7, 8, 5, 2, 4, 9, 3, 1 };
	sort(a);
  }
}
