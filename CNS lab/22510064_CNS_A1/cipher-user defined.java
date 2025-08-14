import java.util.*;

public class cipher {

    // === Caesar Cipher ===
    public static String caesarEncrypt(String text, int shift) {
        StringBuilder result = new StringBuilder();
        for (char c : text.toUpperCase().toCharArray()) {
            if (Character.isLetter(c)) result.append((char) ((c - 'A' + shift + 26) % 26 + 'A'));
            else result.append(c);
        }
        return result.toString();
    }

    public static String caesarDecrypt(String cipher, int shift) {
        return caesarEncrypt(cipher, -shift);
    }

    public static void caesarCryptanalysis(String cipher) {
        System.out.println("\n[+] Caesar Cipher Cryptanalysis (Brute Force):");
        for (int i = 1; i < 26; i++) {
            String attempt = caesarDecrypt(cipher, i);
            System.out.printf("Shift %2d: %s\n", i, attempt);
        }
    }

    // === Playfair Cipher ===
    static char[][] playfairMatrix = new char[5][5];

    public static void generatePlayfairMatrix(String key) {
        key = key.toUpperCase().replace("J", "I").replaceAll("[^A-Z]", "");
        LinkedHashSet<Character> set = new LinkedHashSet<>();
        for (char c : key.toCharArray()) set.add(c);
        for (char c = 'A'; c <= 'Z'; c++) if (c != 'J') set.add(c);
        Iterator<Character> it = set.iterator();
        for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) playfairMatrix[i][j] = it.next();
    }

    private static int[] pos(char c) {
        for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) if (playfairMatrix[i][j] == c) return new int[]{i, j};
        return null;
    }

    public static String playfairEncrypt(String text, String key) {
        generatePlayfairMatrix(key);
        text = text.toUpperCase().replace("J", "I").replaceAll("[^A-Z]", "");
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < text.length(); i += 2) {
            char a = text.charAt(i);
            char b = (i + 1 < text.length()) ? text.charAt(i + 1) : 'X';
            if (a == b) b = 'X';
            int[] p1 = pos(a), p2 = pos(b);
            if (p1[0] == p2[0]) {
                sb.append(playfairMatrix[p1[0]][(p1[1] + 1) % 5]);
                sb.append(playfairMatrix[p2[0]][(p2[1] + 1) % 5]);
            } else if (p1[1] == p2[1]) {
                sb.append(playfairMatrix[(p1[0] + 1) % 5][p1[1]]);
                sb.append(playfairMatrix[(p2[0] + 1) % 5][p2[1]]);
            } else {
                sb.append(playfairMatrix[p1[0]][p2[1]]);
                sb.append(playfairMatrix[p2[0]][p1[1]]);
            }
        }
        return sb.toString();
    }

    public static String playfairDecrypt(String cipher, String key) {
        generatePlayfairMatrix(key);
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < cipher.length(); i += 2) {
            char a = cipher.charAt(i);
            char b = cipher.charAt(i + 1);
            int[] p1 = pos(a), p2 = pos(b);
            if (p1[0] == p2[0]) {
                sb.append(playfairMatrix[p1[0]][(p1[1] + 4) % 5]);
                sb.append(playfairMatrix[p2[0]][(p2[1] + 4) % 5]);
            } else if (p1[1] == p2[1]) {
                sb.append(playfairMatrix[(p1[0] + 4) % 5][p1[1]]);
                sb.append(playfairMatrix[(p2[0] + 4) % 5][p2[1]]);
            } else {
                sb.append(playfairMatrix[p1[0]][p2[1]]);
                sb.append(playfairMatrix[p2[0]][p1[1]]);
            }
        }
        return sb.toString();
    }

    public static void playfairCryptanalysis(String cipher) {
        System.out.println("\n[+] Playfair Cipher Cryptanalysis (Simulated):");
        String[] keys = {"MONARCHY", "KEYWORD", "HELLO", "SECRET"};
        for (String k : keys) {
            String plain = playfairDecrypt(cipher, k);
            System.out.println("Trying key \"" + k + "\": " + plain);
        }
    }

    // === Hill Cipher ===
    static int mod26(int x) {
        x %= 26;
        return (x < 0) ? x + 26 : x;
    }

    static int modInverse(int a) {
        for (int i = 1; i < 26; i++) if ((a * i) % 26 == 1) return i;
        return -1;
    }

    public static String hillEncrypt(String text, int[][] key) {
        text = text.toUpperCase().replaceAll("[^A-Z]", "");
        if (text.length() % 2 != 0) text += "X";
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < text.length(); i += 2) {
            int[] vec = {text.charAt(i) - 'A', text.charAt(i + 1) - 'A'};
            int c1 = mod26(key[0][0] * vec[0] + key[0][1] * vec[1]);
            int c2 = mod26(key[1][0] * vec[0] + key[1][1] * vec[1]);
            sb.append((char) (c1 + 'A')).append((char) (c2 + 'A'));
        }
        return sb.toString();
    }

    public static String hillDecrypt(String cipher, int[][] key) {
        int det = mod26(key[0][0] * key[1][1] - key[0][1] * key[1][0]);
        int inv = modInverse(det);
        if (inv == -1) return "Matrix not invertible.";
        int[][] invKey = {
                {mod26(inv * key[1][1]), mod26(-inv * key[0][1])},
                {mod26(-inv * key[1][0]), mod26(inv * key[0][0])}
        };
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < cipher.length(); i += 2) {
            int[] vec = {cipher.charAt(i) - 'A', cipher.charAt(i + 1) - 'A'};
            int p1 = mod26(invKey[0][0] * vec[0] + invKey[0][1] * vec[1]);
            int p2 = mod26(invKey[1][0] * vec[0] + invKey[1][1] * vec[1]);
            sb.append((char) (p1 + 'A')).append((char) (p2 + 'A'));
        }
        return sb.toString();
    }

    public static void hillCryptanalysis(String cipher) {
        System.out.println("\n[+] Hill Cipher Cryptanalysis (Simulated keys):");
        int[][][] keys = {
                {{3, 3}, {2, 5}}, {{1, 2}, {3, 5}}, {{7, 8}, {11, 11}}
        };
        for (int[][] k : keys) {
            String attempt = hillDecrypt(cipher, k);
            System.out.println("Trying key: " + Arrays.deepToString(k) + " => " + attempt);
        }
    }

    // === Vigenere Cipher ===
    public static String vigenereEncrypt(String text, String key) {
        text = text.toUpperCase().replaceAll("[^A-Z]", "");
        key = key.toUpperCase();
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < text.length(); i++) {
            int t = text.charAt(i) - 'A';
            int k = key.charAt(i % key.length()) - 'A';
            sb.append((char) ((t + k) % 26 + 'A'));
        }
        return sb.toString();
    }

    public static String vigenereDecrypt(String cipher, String key) {
        cipher = cipher.toUpperCase();
        key = key.toUpperCase();
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < cipher.length(); i++) {
            int c = cipher.charAt(i) - 'A';
            int k = key.charAt(i % key.length()) - 'A';
            sb.append((char) ((c - k + 26) % 26 + 'A'));
        }
        return sb.toString();
    }

    public static void vigenereCryptanalysis(String cipher) {
        System.out.println("\n[+] Vigenère Cipher Cryptanalysis (Try keys):");
        String[] keys = {"KEY", "HELLO", "WORLD"};
        for (String k : keys) {
            String attempt = vigenereDecrypt(cipher, k);
            System.out.println("Trying key \"" + k + "\": " + attempt);
        }
    }

    // === Main ===
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter plaintext: ");
        String plain = sc.nextLine();

        System.out.print("Enter Caesar Cipher shift: ");
        int caesarShift = sc.nextInt();
        sc.nextLine(); // consume newline

        System.out.print("Enter Playfair key: ");
        String playfairKey = sc.nextLine();

        System.out.print("Enter Hill Cipher 2x2 key matrix values (4 integers): ");
        int[][] hillKey = new int[2][2];
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                hillKey[i][j] = sc.nextInt();
        sc.nextLine(); // consume newline

        System.out.print("Enter Vigenère Cipher key: ");
        String vigenereKey = sc.nextLine();

        // Caesar
        String caesarEnc = caesarEncrypt(plain, caesarShift);
        System.out.println("\nCaesar Encrypted: " + caesarEnc);
        System.out.println("Caesar Decrypted: " + caesarDecrypt(caesarEnc, caesarShift));
        caesarCryptanalysis(caesarEnc);

        // Playfair
        String playfairEnc = playfairEncrypt(plain, playfairKey);
        System.out.println("\nPlayfair Encrypted: " + playfairEnc);
        System.out.println("Playfair Decrypted: " + playfairDecrypt(playfairEnc, playfairKey));
        playfairCryptanalysis(playfairEnc);

        // Hill
        String hillEnc = hillEncrypt(plain, hillKey);
        System.out.println("\nHill Encrypted: " + hillEnc);
        System.out.println("Hill Decrypted: " + hillDecrypt(hillEnc, hillKey));
        hillCryptanalysis(hillEnc);

        // Vigenère
        String vigenereEnc = vigenereEncrypt(plain, vigenereKey);
        System.out.println("\nVigenère Encrypted: " + vigenereEnc);
        System.out.println("Vigenère Decrypted: " + vigenereDecrypt(vigenereEnc, vigenereKey));
        vigenereCryptanalysis(vigenereEnc);

        sc.close();
    }
}
