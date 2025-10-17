import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Scanner;

public class SHA1Demo {

    // convert bytes to hex string
    private static String bytesToHex(byte[] bytes) {
        StringBuilder sb = new StringBuilder(bytes.length * 2);
        for (byte b : bytes) {
            sb.append(String.format("%02x", b & 0xff));
        }
        return sb.toString();
    }

    private static String sha1Hex(String input) throws NoSuchAlgorithmException {
        MessageDigest md = MessageDigest.getInstance("SHA-1");
        byte[] digest = md.digest(input.getBytes(StandardCharsets.UTF_8));
        return bytesToHex(digest);
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in, StandardCharsets.UTF_8.name());
        try {
            System.out.println("SHA-1 Message Digest Calculator (Java)");
            System.out.println("--------------------------------------");
            System.out.print("Enter text: ");
            String text = sc.nextLine();

            String digest = sha1Hex(text);
            System.out.println("\nInput: \"" + text + "\"");
            System.out.println("SHA-1 digest (hex): " + digest);

            // Avalanche demonstration prompt
            System.out.print("\nEnter a slightly modified version of the text (or press Enter to auto-modify): ");
            String modified = sc.nextLine();
            if (modified.isEmpty()) {
                // auto modify: flip case of first character if exists, else add 'a'
                if (text.length() > 0) {
                    char c = text.charAt(0);
                    char flipped = Character.isUpperCase(c) ? Character.toLowerCase(c) : Character.toUpperCase(c);
                    modified = flipped + text.substring(1);
                } else {
                    modified = "a";
                }
                System.out.println("Auto-modified input: \"" + modified + "\"");
            }

            String digest2 = sha1Hex(modified);
            System.out.println("\nModified Input: \"" + modified + "\"");
            System.out.println("SHA-1 digest (hex): " + digest2);

            // quick percent of differing hex chars (informal measure)
            int diffs = 0;
            int len = Math.min(digest.length(), digest2.length());
            for (int i = 0; i < len; i++) if (digest.charAt(i) != digest2.charAt(i)) diffs++;
            System.out.printf("\nHex characters different: %d out of %d (%.1f%%)\n", diffs, len, 100.0 * diffs / len);

            // show some known test vectors (optional)
            System.out.println("\nSome known SHA-1 digests:");
            System.out.println("  \"\" -> da39a3ee5e6b4b0d3255bfef95601890afd80709");
            System.out.println("  \"hello world\" -> 2aae6c35c94fcfb415dbe95f408b9ce91ee846ed");
            System.out.println("  \"The quick brown fox jumps over the lazy dog\" -> 2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");

        } catch (NoSuchAlgorithmException e) {
            System.err.println("SHA-1 algorithm not available: " + e.getMessage());
        } finally {
            sc.close();
        }
    }
}
