public class HeapSort {

  int heapify_n = 0;

  private int leftChild(int i) {
    return 2 * i + 1; //index
  }
	
  private int rightChild(int i) {
    return 2 * i + 2; //index
  }
  
  private void swap(int[] a, int index1, int index2)
  {
    if (index1 < a.length && index2 < a.length && index1 != index2)
    {
      int copy = a[index1];
      a[index1] = a[index2];
      a[index2] = copy;
	  //System.out.println(a[index1] + " " + a[index2]);
    }
  }

  // Korrigiere einen inkorrekten Wert an Stelle i
  // (siehe Vorlesung)
  private void min_heapify(int[] a, int i) {
    int rootIndex = i;
    int leftIndex = leftChild(i);
    int rightIndex = rightChild(i);
    int heapSize = heapify_n;
  
    if (rightIndex < heapSize && a[rightIndex] < a[rootIndex]) 
      rootIndex = rightIndex; 
  
    if (leftIndex < heapSize && a[leftIndex] < a[rootIndex]) 
        rootIndex = leftIndex;
    
    if (rootIndex != i) { 
      	swap(a, rootIndex, i);
		min_heapify(a, rootIndex); 
    }
  }
  
  // Verwandelt das Array in einen Heap
  // (siehe Vorlesung)
  private void build_min_heap(int[] a) {
    int heap_size = a.length;
    for (int i = heap_size/2 - 1; i >= 0; --i) {
      min_heapify(a, 0);
    }
  }
  
  // HeapSort
  public void sort(int[] a) {
    build_min_heap(a);
    int n = a.length;
    
    for (int i = n - 1; n >= 0; n--) {
      swap(a, i, 0); //Wurzel und Ende vertauschen
	  heapify_n = i;
      min_heapify(a, 0);
    }

  }

  public static void main(String[] args) {
	int[] a = new int[] {7, 3, 1, 9, 5, 6, 4, 2, 8};

	HeapSort h = new HeapSort();
	h.sort(a);

	System.out.println("\n");
	for (int i = 0; i < a.length; i++) {
		System.out.print(a[i] + " ");
	}
	System.out.println("\n");
  }
}
