import java.math.BigInteger;
import java.util.Scanner;

public class RSAImplementation {
    private static BigInteger gcd(BigInteger a, BigInteger b) {
        return b.equals(BigInteger.ZERO) ? a : gcd(b, a.mod(b));
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter prime number p: ");
        BigInteger p = sc.nextBigInteger();

        System.out.print("Enter prime number q: ");
        BigInteger q = sc.nextBigInteger();

        BigInteger n = p.multiply(q);
        BigInteger phi = (p.subtract(BigInteger.ONE)).multiply(q.subtract(BigInteger.ONE));
        System.out.println("\nn = p * q = " + n);
        System.out.println("phi(n) = " + phi);

        System.out.print("\nEnter public exponent e (1 < e < phi(n) and gcd(e,phi)=1): ");
        BigInteger e = sc.nextBigInteger();
        while (e.compareTo(BigInteger.ONE) <= 0 ||
               e.compareTo(phi) >= 0 ||
               !gcd(e, phi).equals(BigInteger.ONE)) {
            System.out.print("Invalid e. Enter again: ");
            e = sc.nextBigInteger();
        }

        BigInteger d = e.modInverse(phi);
        System.out.println("\nPublic Key  : (e = " + e + ", n = " + n + ")");
        System.out.println("Private Key : (d = " + d + ", n = " + n + ")");

        System.out.print("\nEnter plaintext message as an integer (0 < M < n): ");
        BigInteger M = sc.nextBigInteger();
        while (M.compareTo(BigInteger.ZERO) <= 0 || M.compareTo(n) >= 0) {
            System.out.print("Message out of range. Enter again: ");
            M = sc.nextBigInteger();
        }

        BigInteger C = M.modPow(e, n);
        System.out.println("Encrypted Ciphertext: " + C);

        BigInteger decrypted = C.modPow(d, n);
        System.out.println("Decrypted Plaintext : " + decrypted);

        if (M.equals(decrypted)) {
            System.out.println("Decryption successful! Message matches!");
        } else {
            System.out.println("Decryption failed.");
        }

        sc.close();
    }
}
