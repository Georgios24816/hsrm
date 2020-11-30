public class HasDups {

    static boolean hasDups(int[] xs) {
      int n = xs.length;
      
      for (int i = 0; i < n; i++) {
        if (linearSearch(xs, i + 1, xs[i])) {
          return true;
        }
      }
      
     return false;
    }

    private static boolean linearSearch(int[] xs, int start, int v) {
      int j = start;
      int n = xs.length;
      
      while (j < n) {
        if (xs[j] == v) {
          return true;
        }
      	
		j++;
      }
      
      
      return false;
    }

    static boolean hasDupsFaster(int[] xs) {
      int i1 = 0;
	  int i2 = 1;
	  int n = xs.length;

	  if (n > 1) {
		while (i2 < n - 1 & xs[i1] != xs[i2]) {
		  i1++;
		  i2++;
	  	}

		if (xs[i1] == xs[i2]) {
			return true;
		}

		if (i2 != n - 1) {
		  return true;
		}
	  }

	  return false;
    }


    // as requested on the exercise sheet, we
    // run a duplicate check and record runtime.
    public static void runTest(int[] xs) {
    boolean result;
    long startTime;
    long endTime;

    // run slow version
    startTime = System.currentTimeMillis();
    result = hasDups(xs);
    endTime = System.currentTimeMillis();
    System.out.format(
        "[QUADRATIC] %d values: %d ms. Found duplicate? %b\n",
        xs.length, (endTime - startTime), result);

    // run fast version
    startTime = System.currentTimeMillis();
    result = hasDupsFaster(xs);
    endTime = System.currentTimeMillis();
    System.out.format(
        "[LINEAR] %d values: %d ms. Found duplicate? %b\n",
        xs.length, (endTime - startTime), result);
    }
	
  public static void main(String[]args) {
	int n0 = 1000;
	int[] x0 = new int [n0];
	
	int n1 = 10000;
	int[] x1 = new int [n1];

	int n2 = 100000;
	int[] x2 = new int [n2];

	int n3 = 200000;
	int[] x3 = new int [n3];

	for (int i = 0; i < n0; i++) {
	  x0[i] = i;
	}

	for (int i = 0; i < n1; i++) {
	  x1[i] = i;
	}

	for (int i = 0; i < n2; i++) {
	  x2[i] = i;
	}

	for (int i = 0; i < n3; i++) {
	  x3[i] = i;
	}

	runTest(x0);
	runTest(x1);
	runTest(x2);
	runTest(x3);
  }
}





