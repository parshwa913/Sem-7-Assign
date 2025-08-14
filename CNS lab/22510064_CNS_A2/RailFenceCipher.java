import java.util.Scanner;

public class RailFenceCipher {

    public static String encrypt(String text, int key) {
        StringBuilder[] rails = new StringBuilder[key];
        for (int i = 0; i < key; i++) rails[i] = new StringBuilder();

        int dir = 1, row = 0;
        for (char c : text.toCharArray()) {
            rails[row].append(c);
            row += dir;
            if (row == key - 1) dir = -1;
            else if (row == 0) dir = 1;
        }
        StringBuilder result = new StringBuilder();
        for (StringBuilder rail : rails) result.append(rail);
        return result.toString();
    }

    public static String decrypt(String cipher, int key) {
        int[] railLengths = new int[key];
        int row = 0, dir = 1;

        for (int i = 0; i < cipher.length(); i++) {
            railLengths[row]++;
            row += dir;
            if (row == key - 1) dir = -1;
            else if (row == 0) dir = 1;
        }

        String[] rails = new String[key];
        int start = 0;
        for (int i = 0; i < key; i++) {
            rails[i] = cipher.substring(start, start + railLengths[i]);
            start += railLengths[i];
        }

        int[] railIndices = new int[key];
        StringBuilder result = new StringBuilder();
        row = 0; dir = 1;
        for (int i = 0; i < cipher.length(); i++) {
            result.append(rails[row].charAt(railIndices[row]++));
            row += dir;
            if (row == key - 1) dir = -1;
            else if (row == 0) dir = 1;
        }
        return result.toString();
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter text: ");
        String text = sc.nextLine().replaceAll("\\s+", "").toUpperCase();

        System.out.print("Enter key (number of rails): ");
        int key = sc.nextInt();

        String encrypted = encrypt(text, key);
        System.out.println("Encrypted: " + encrypted);

        String decrypted = decrypt(encrypted, key);
        System.out.println("Decrypted: " + decrypted);

        sc.close();
    }
}
