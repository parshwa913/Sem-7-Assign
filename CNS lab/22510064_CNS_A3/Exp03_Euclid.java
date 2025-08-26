import java.util.*;

public class Exp03_Euclid {
    static class EResult {
        long gcd, x, y;
        EResult(long g, long x, long y) { this.gcd = g; this.x = x; this.y = y; }
    }

    // Standard Euclidean algorithm
    static long gcd(long a, long b) {
        a = Math.abs(a); b = Math.abs(b);
        while (b != 0) {
            long t = a % b;
            a = b; b = t;
        }
        return a;
    }

    // Extended Euclidean algorithm: returns (gcd, x, y)
    static EResult egcd(long a, long b) {
        if (b == 0) return new EResult(Math.abs(a), a >= 0 ? 1 : -1, 0);
        EResult r = egcd(b, a % b);
        long g = r.gcd;
        long x1 = r.y;
        long y1 = r.x - (a / b) * r.y;
        return new EResult(g, x1, y1);
    }

    // Modular inverse of a mod m (if gcd(a,m)=1)
    static Optional<Long> modInverse(long a, long m) {
        EResult r = egcd(a, m);
        if (r.gcd != 1) return Optional.empty();
        long inv = r.x % m;
        if (inv < 0) inv += m;
        return Optional.of(inv);
    }

    public static void main(String[] args) {
        try (Scanner sc = new Scanner(System.in)) {
            System.out.println("=== Euclid & Extended Euclid ===");
            System.out.print("Enter a: ");
            long a = sc.nextLong();
            System.out.print("Enter b (or modulus m for inverse): ");
            long b = sc.nextLong();

            long g = gcd(a, b);
            EResult r = egcd(a, b);
            System.out.println("gcd(" + a + ", " + b + ") = " + g);
            System.out.println("Bezout coefficients (x, y): x=" + r.x + ", y=" + r.y);
            System.out.println(a + "*(" + r.x + ") + " + b + "*(" + r.y + ") = " + (a*r.x + b*r.y));

            if (g == 1) {
                Optional<Long> invAmodB = modInverse(a, b);
                invAmodB.ifPresent(inv ->
                    System.out.println("Inverse of " + a + " mod " + b + " = " + inv)
                );
            } else {
                System.out.println("No modular inverse exists for " + a + " mod " + b + " (gcd != 1).");
            }
        }
    }
}
