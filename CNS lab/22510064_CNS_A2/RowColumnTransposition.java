import java.util.*;

public class RowColumnTransposition {

    // Generate column read order based on the key sequence
    public static int[] getOrder(int[] key) {
        int[] order = new int[key.length];
        Integer[] idx = new Integer[key.length];
        for (int i = 0; i < key.length; i++) idx[i] = i;

        Arrays.sort(idx, Comparator.comparingInt(i -> key[i]));

        for (int i = 0; i < key.length; i++) {
            order[i] = idx[i];
        }
        return order;
    }

    public static String encrypt(String text, int[] key) {
        int cols = key.length;
        int rows = (int) Math.ceil((double) text.length() / cols);
        char[][] matrix = new char[rows][cols];
        int index = 0;

        // Fill row-wise
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                matrix[r][c] = (index < text.length()) ? text.charAt(index++) : 'X';
            }
        }

        StringBuilder result = new StringBuilder();
        int[] order = getOrder(key);

        // Read columns in sorted key order
        for (int colIndex : order) {
            for (int r = 0; r < rows; r++) {
                result.append(matrix[r][colIndex]);
            }
        }
        return result.toString();
    }

    public static String decrypt(String cipher, int[] key) {
        int cols = key.length;
        int rows = (int) Math.ceil((double) cipher.length() / cols);
        char[][] matrix = new char[rows][cols];
        int index = 0;

        int[] order = getOrder(key);

        // Fill columns in sorted key order
        for (int colIndex : order) {
            for (int r = 0; r < rows; r++) {
                matrix[r][colIndex] = cipher.charAt(index++);
            }
        }

        StringBuilder result = new StringBuilder();
        // Read row-wise
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                result.append(matrix[r][c]);
            }
        }
        return result.toString().replaceAll("X+$", "");
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter text: ");
        String text = sc.nextLine().replaceAll("\\s+", "").toUpperCase();

        System.out.print("Enter key length: ");
        int keyLength = sc.nextInt();

        int[] key = new int[keyLength];
        System.out.println("Enter key sequence (1-based column order): ");
        for (int i = 0; i < keyLength; i++) {
            key[i] = sc.nextInt();
        }

        String encrypted = encrypt(text, key);
        System.out.println("Encrypted: " + encrypted);

        String decrypted = decrypt(encrypted, key);
        System.out.println("Decrypted: " + decrypted);

        sc.close();
    }
}
