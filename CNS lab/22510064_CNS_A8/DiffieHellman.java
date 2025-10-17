import java.math.BigInteger;
import java.security.SecureRandom;
import java.util.Scanner;

/**
 * Diffie-Hellman demonstration in Java using BigInteger.
 *
 * Features:
 * - Interactive: accepts p, g, and private keys (or 'r' for random).
 * - Uses BigInteger.modPow for modular exponentiation.
 * - Optional brute-force discrete-log solver for small p (to simulate Eve).
 *
 * Note: For production use, use standardized primes and authenticated DH (e.g., within TLS).
 */
public class DiffieHellman {
    private static final SecureRandom random = new SecureRandom();

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("Diffie-Hellman Key Exchange Demo (Java)");
        System.out.println("---------------------------------------------------");

        System.out.print("Enter prime p (decimal): ");
        BigInteger p = new BigInteger(sc.next());

        if (!p.isProbablePrime(20)) {
            System.out.println("Warning: p is not strongly prime according to quick test (probablePrime).");
        }

        System.out.print("Enter generator g (decimal): ");
        BigInteger g = new BigInteger(sc.next());

        BigInteger a = readPrivate("Alice", sc, p);
        BigInteger b = readPrivate("Bob", sc, p);

        // Compute public keys
        BigInteger A = g.modPow(a, p); // Alice's public
        BigInteger B = g.modPow(b, p); // Bob's public

        System.out.println("\nPublic values:");
        System.out.println("A (Alice's public key) = g^a mod p = " + A);
        System.out.println("B (Bob's public key)   = g^b mod p = " + B);

        // Compute shared secrets
        BigInteger K_A = B.modPow(a, p);
        BigInteger K_B = A.modPow(b, p);

        System.out.println("\nShared secret computed by Alice: " + K_A);
        System.out.println("Shared secret computed by Bob:   " + K_B);
        System.out.println("Shared keys equal? " + K_A.equals(K_B));

        // Optional: attempt brute force discrete log for small p
        if (p.bitLength() <= 20) { // small p -> feasible to brute force
            System.out.println("\nEve (attacker) simulation: trying to recover Alice's private key a by brute force...");
            BigInteger found = bruteForcePrivateKey(A, g, p);
            if (found != null) {
                System.out.println("Eve found a = " + found + " (verifies: g^a mod p = " + g.modPow(found, p) + ")");
                System.out.println("Eve can compute shared secret K = g^(ab) mod p = " + g.modPow(found.multiply(b), p).mod(p));
            } else {
                System.out.println("Eve failed to find a (unexpected).");
            }
        } else {
            System.out.println("\nEve simulation skipped: p is too large for brute-force in this demo.");
            System.out.println("For real-world p (e.g., 2048-bit), discrete log brute force is infeasible.");
        }

        sc.close();
    }

    private static BigInteger readPrivate(String who, Scanner sc, BigInteger p) {
        while (true) {
            System.out.print("Enter " + who + "'s private key (decimal) or 'r' for random: ");
            String s = sc.next();
            if (s.equalsIgnoreCase("r")) {
                // generate random 1 <= x <= p-2
                BigInteger max = p.subtract(BigInteger.valueOf(2));
                BigInteger x;
                do {
                    x = new BigInteger(max.bitLength(), random);
                } while (x.compareTo(BigInteger.ONE) < 0 || x.compareTo(max) > 0);
                System.out.println(who + " private key (randomly chosen): " + x);
                return x;
            } else {
                try {
                    BigInteger x = new BigInteger(s);
                    if (x.compareTo(BigInteger.ONE) < 0 || x.compareTo(p.subtract(BigInteger.ONE)) > 0) {
                        System.out.println("Private key must satisfy 1 <= key <= p-2. Try again.");
                        continue;
                    }
                    return x;
                } catch (NumberFormatException ex) {
                    System.out.println("Invalid number. Try again.");
                }
            }
        }
    }

    // Brute force discrete log: find x such that g^x mod p == publicKey.
    // Only feasible for small p; used for demonstration of insecurity with small primes.
    private static BigInteger bruteForcePrivateKey(BigInteger publicKey, BigInteger g, BigInteger p) {
        BigInteger x = BigInteger.ZERO;
        BigInteger limit = p; // search 0..p-1
        while (x.compareTo(limit) < 0) {
            if (g.modPow(x, p).equals(publicKey)) {
                return x;
            }
            x = x.add(BigInteger.ONE);
        }
        return null;
    }
}
