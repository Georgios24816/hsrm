package de.hsrm.cs.ads;
import java.util.List;
import java.util.LinkedList;

/*
theoretische Aufgaben:
1)
	1) 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 
  	2) 2 3 — 5 — 7 — 9 —— 11 —— 13 —— 15 —— 17
	3) 2 3 — 5 — 7 — — —— 11 —— 13 —— —— —— 17 
2)
Der Algorithmus terminiert, denn er fügt alle Primzahlen zu einer Liste 
hinzu und es gibt eine endliche Anzahl von Primzahlen.

Der Algorithmus ist determiniert, weil sich die Primzahlen nicht ändern.

Der Algorithmus ist deterministisch, denn alle Zwischenergebnisse sind
gleichen Startbedigungen immer gleich.

Der Algorithmus ist korrekt, denn er ist terminierend und liefert das
gewollte Ergebnis zurück, nämlich eine Liste mit Primzahlen bis m
*/

public class Sieve {
  static boolean[] sieve(int n) {
    boolean[] result = new boolean[n + 1];
    List<Integer> primes = primesUpTo(n);
    
    for (int i = 0; i < result.length; i++) {
      if (primes.contains(i)) {
        result[i] = true;
      } else {
        result[i] = false;
      }
    }
    
    return result;
  }
  
  static List<Integer> primesUpTo(int n){
    List<Integer> result = new LinkedList<>();
    boolean[] isPrime = new boolean[n+1];
    for (int i = 2; i <= n; i++) {
      isPrime[i] = true;
    }


    for (int factor = 2; factor*factor <= n; factor++) {
      if (isPrime[factor]) {
        for (int j = factor; factor*j <= n; j++) {
          isPrime[factor*j] = false;
        }
      }
    }

    for (int i = 2; i <= n; i++) {
      if (isPrime[i]) {
        result.add(i);
      }
    }
    
    return result;
  }
}
