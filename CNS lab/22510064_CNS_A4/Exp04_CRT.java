import java.util.*;

public class Exp04_CRT {
    static class EG { long g,x,y; EG(long g,long x,long y){this.g=g;this.x=x;this.y=y;} }
    static EG egcd(long a, long b){
        if(b==0) return new EG(Math.abs(a), a>=0?1:-1, 0);
        EG r = egcd(b, a%b);
        long g = r.g, x = r.y, y = r.x - (a/b)*r.y;
        return new EG(g, x, y);
    }
    static Optional<Long> inv(long a, long m){
        EG r = egcd(a, m);
        if(r.g!=1) return Optional.empty();
        long v = r.x % m; if(v<0) v+=m; return Optional.of(v);
    }

    public static void main(String[] args){
        try (Scanner sc = new Scanner(System.in)) {
        System.out.println("=== Chinese Remainder Theorem ===");
        System.out.print("Enter number of congruences k: ");
        int k = sc.nextInt();
        long[] a = new long[k];
        long[] n = new long[k];
        for(int i=0;i<k;i++){
            System.out.print("Enter a" + (i+1) + ": ");
            a[i] = sc.nextLong();
            System.out.print("Enter n" + (i+1) + " (must be pairwise coprime): ");
            n[i] = sc.nextLong();
        }

        // Check pairwise coprime
        for(int i=0;i<k;i++){
            for(int j=i+1;j<k;j++){
                long g = gcd(n[i], n[j]);
                if(g != 1){
                    System.out.println("Error: n" + (i+1) + " and n" + (j+1) + " are not coprime (gcd="+g+").");
                    return;
                }
            }
        }

        long N = 1;
        for(long ni : n) N *= ni;

        long x = 0;
        for(int i=0;i<k;i++){
            long Ni = N / n[i];
            Optional<Long> invNi = inv(Ni % n[i], n[i]);
            if(invNi.isEmpty()){
                System.out.println("No inverse for Ni mod n" + (i+1) + ", aborting.");
                return;
            }
            long term = (a[i] % N + N) % N;
            x = (x + term * Ni % N * invNi.get() % N) % N;
        }
        if(x<0) x += N;
        System.out.println("Smallest non-negative solution x = " + x);
        System.out.println("Solution is unique modulo N = " + N);
    }
    }
    static long gcd(long a, long b){
        a = Math.abs(a); b = Math.abs(b);
        while(b!=0){ long t=a%b; a=b; b=t; }
        return a;
    }
}
