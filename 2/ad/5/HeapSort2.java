package de.hsrm.cs.ads;

public class HeapSort {

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
    }
  }

  // Korrigiere einen inkorrekten Wert an Stelle i
  // (siehe Vorlesung)
  private void min_heapify(int[] a, int i) {
    int rootIndex = i;
    int leftIndex = leftChild(i);
    int rightIndex = rightChild(i);
    int heapSize = a.length;
  
    if (rightIndex < heapSize && a[rightIndex] < a[rootIndex]) 
      rootIndex = rightIndex; 
  
    if (leftIndex < heapSize && a[leftIndex] < arr[rootIndex]) 
        rootIndex = leftIndex;
    
    if (rootIndex != i) { 
      int copy = a[rootIndex]; 
      a[rootIndex] = a[rootIndex]; 
      a[rootIndex] = copy; 
      min_heapify(a, smallest); 
    }
         
  }
  
  // Verwandelt das Array in einen Heap
  // (siehe Vorlesung)
  private void build_min_heap(int[] a) {
    heap_size = a.length;
    for (int i = heap_size/2; i >= 0; --i) {
      min_heapify(a, i);
    }
  }
  
  // HeapSort
  public void sort(int[] a) {
    build_min_heap(a);
    int n = a.length;
    
    for (int i = n - 1; n >= 0; n--) {
      swap(a, i, 0); //Wurzel und Ende vertauschen
      min_heapify(a, i);
    }
  }
}
