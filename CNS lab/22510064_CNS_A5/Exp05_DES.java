import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;
import java.util.Base64;
import java.util.Scanner;

public class Exp05_DES {

    static byte[] hexToBytes(String s) {
        if (s.length() % 2 != 0) throw new IllegalArgumentException("Hex length must be even.");
        byte[] out = new byte[s.length()/2];
        for (int i=0;i<out.length;i++) {
            int hi = Character.digit(s.charAt(2*i),16);
            int lo = Character.digit(s.charAt(2*i+1),16);
            if (hi<0 || lo<0) throw new IllegalArgumentException("Invalid hex: " + s);
            out[i] = (byte)((hi<<4) | lo);
        }
        return out;
    }
    static String bytesToHex(byte[] b) {
        StringBuilder sb = new StringBuilder();
        for (byte x: b) sb.append(String.format("%02X", x));
        return sb.toString();
    }

    static byte[] parseKey(String keyInput, boolean isHex) {
        byte[] key;
        if (isHex) {
            key = hexToBytes(keyInput);
        } else {
            key = keyInput.getBytes(); // platform default charset; OK for ASCII lab use
        }
        if (key.length != 8)
            throw new IllegalArgumentException("DES key must be exactly 8 bytes (got "+key.length+").");
        return key;
    }

    public static void main(String[] args) throws Exception {
        Scanner sc = new Scanner(System.in);
        System.out.println("=== DES Encrypt/Decrypt ===");
        System.out.print("Key input type? (hex/ascii): ");
        String kt = sc.next().trim().toLowerCase();
        boolean hexKey = kt.equals("hex");
        sc.nextLine(); // consume endline

        System.out.print("Enter key (" + (hexKey ? "16 hex chars" : "8 ASCII chars") + "): ");
        String keyStr = sc.nextLine().trim();
        byte[] keyBytes = parseKey(keyStr, hexKey);
        SecretKeySpec key = new SecretKeySpec(keyBytes, "DES");

        System.out.print("Plaintext input type? (hex/text): ");
        String ptType = sc.next().trim().toLowerCase();
        sc.nextLine();

        if (ptType.equals("hex")) {
            System.out.print("Enter plaintext HEX (exactly 16 hex chars => 8 bytes): ");
            String phex = sc.nextLine().trim();
            byte[] pt = hexToBytes(phex);
            if (pt.length != 8) {
                System.out.println("For hex mode, plaintext must be 8 bytes (16 hex chars).");
                sc.close();
                return;
            }
            Cipher c = Cipher.getInstance("DES/ECB/NoPadding");
            c.init(Cipher.ENCRYPT_MODE, key);
            byte[] ct = c.doFinal(pt);
            System.out.println("Ciphertext (HEX) = " + bytesToHex(ct));

            c.init(Cipher.DECRYPT_MODE, key);
            byte[] dec = c.doFinal(ct);
            System.out.println("Decrypted PT (HEX) = " + bytesToHex(dec));
        } else {
            System.out.print("Enter plaintext TEXT: ");
            String ptext = sc.nextLine();
            Cipher c = Cipher.getInstance("DES/ECB/PKCS5Padding");
            c.init(Cipher.ENCRYPT_MODE, key);
            byte[] ct = c.doFinal(ptext.getBytes());
            String b64 = Base64.getEncoder().encodeToString(ct);
            System.out.println("Ciphertext (Base64) = " + b64);

            c.init(Cipher.DECRYPT_MODE, key);
            byte[] dec = c.doFinal(Base64.getDecoder().decode(b64));
            System.out.println("Decrypted PT (TEXT) = " + new String(dec));
        }
         sc.close();
    }
}
