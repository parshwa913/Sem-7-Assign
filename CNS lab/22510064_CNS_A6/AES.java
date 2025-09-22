import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import java.util.Base64;
import java.util.Scanner;

public class AES {

    // Method to generate AES key of given size
    public static SecretKey generateKey(int n) throws Exception {
        KeyGenerator keyGen = KeyGenerator.getInstance("AES");
        keyGen.init(n); // key size: 128, 192, or 256
        return keyGen.generateKey();
    }

    // Convert string key to SecretKeySpec
    public static SecretKeySpec getKeyFromString(String key) throws Exception {
        byte[] keyBytes = key.getBytes("UTF-8");
        // Use only first 16 bytes for 128-bit key
        byte[] keyBytes16 = new byte[16];
        System.arraycopy(keyBytes, 0, keyBytes16, 0, Math.min(keyBytes.length, 16));
        return new SecretKeySpec(keyBytes16, "AES");
    }

    // Encrypt text
    public static String encrypt(String plainText, SecretKeySpec secretKey) throws Exception {
        Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
        cipher.init(Cipher.ENCRYPT_MODE, secretKey);
        byte[] cipherText = cipher.doFinal(plainText.getBytes("UTF-8"));
        return Base64.getEncoder().encodeToString(cipherText);
    }

    // Decrypt text
    public static String decrypt(String cipherText, SecretKeySpec secretKey) throws Exception {
        Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
        cipher.init(Cipher.DECRYPT_MODE, secretKey);
        byte[] plainText = cipher.doFinal(Base64.getDecoder().decode(cipherText));
        return new String(plainText, "UTF-8");
    }

    public static void main(String[] args) {
        try (Scanner sc = new Scanner(System.in)) {
            System.out.println("Enter plaintext message:");
            String plainText = sc.nextLine();

            System.out.println("Enter secret key (any string, min 16 characters recommended):");
            String keyInput = sc.nextLine();
            SecretKeySpec secretKey = getKeyFromString(keyInput);

            // Encryption
            String cipherText = encrypt(plainText, secretKey);
            System.out.println("\nEncrypted Text (Base64): " + cipherText);

            // Decryption
            String decryptedText = decrypt(cipherText, secretKey);
            System.out.println("Decrypted Text: " + decryptedText);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
