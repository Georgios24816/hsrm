public class CocktailShakerSort {
	public static void sort(int[] a) {
		boolean swapped = true; 
        int start = 0; 
        int end = a.length; 
  
        while (swapped == true) { 
            swapped = false; //zurücksetzen für nächsten Durchlauf
  
			//Durchlauf von Anfang bis Ende
            for (int i = start; i < end - 1; ++i) { 
                if (a[i] > a[i + 1]) { 
                    int temp = a[i]; 
                    a[i] = a[i + 1]; 
                    a[i + 1] = temp; 
                    swapped = true; 
                } 
            } 
  
            if (swapped == false) 
                break; //schon sortiert
  
            swapped = false; //ansonsten zurücksetzen für neuen Durchlauf
  
            end--;//weil das größte Element an der richtigen Stelle ist
		   		  //wegen der oberen Schleife	
  
			//Durchlauf von Ende nach Anfang
            for (int i = end - 1; i >= start; i--) { 
                if (a[i] > a[i + 1]) { 
                    int temp = a[i]; 
                    a[i] = a[i + 1]; 
                    a[i + 1] = temp; 
                    swapped = true; 
                } 
            } 
  
            start = start + 1; 
        } 
	}
}
