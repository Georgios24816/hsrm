import java.util.Random;

public class MergeSort {

  // buffer for merge operation.
  int[] b;
  
  public void sort(int[] a) {
    sortRange(a, 0, a.length - 1);
  }
  
  public void sortRange(int[] a, int l, int r) {  
    if (l < r) {
      int q = (l + r) / 2;

      sortRange(a, l, q);
      sortRange(a, q + 1, r);
      merge(a, l, q, r);
    }
  
  }
  
  public void merge(int[] a, int l, int q, int r) {
    b = new int[a.length];
    int i, j;
    
    for (i = l; i <= q; i++) {
      b[i] = a[i];
    }
    
    for (j = q + 1; j <= r; j++) {
      b[r + q + 1 - j] = a[j];
    }
    
    i = l;
    j = r;
    
    for (int k = l; k <= r; k++) {
      if (b[i] <= b[j]) {
        a[k] = b[i];
        i++;
      } else {
        a[k] = b[j];
        j--;
      }
    }
  }

  
  public static void swap(int[] a, int pos1, int pos2) {
    int tmp = a[pos1];
    a[pos1] = a[pos2];
    a[pos2] = tmp;
  }

  // always the same pseudo random numbers!
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
    MergeSort sort = new MergeSort();
    startTime = System.currentTimeMillis();
    sort.sort(a);
    endTime = System.currentTimeMillis();
    System.out.format(
        "[n=%d] : %d ms.\n",
        n, (endTime - startTime));
  }

  public static void main(String[] args) {
	  runTest(10 * 100000);
	  runTest(5 * 10 * 100000);
	  runTest(10 * 10 * 100000);
  }
  
}
