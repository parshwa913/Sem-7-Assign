import java.nio.charset.StandardCharsets;
import java.security.*;
import java.security.spec.*;
import java.util.Base64;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

public class DSASignature {
    private static final Logger logger = Logger.getLogger(DSASignature.class.getName());

    private KeyPair keyPair;

    public DSASignature() {
        try {
            generateKeys(2048);
        } catch (Exception e) {
            logger.log(Level.SEVERE, "Failed to generate keys on startup", e);
        }
    }

    public void generateKeys(int keySize) throws NoSuchAlgorithmException {
        KeyPairGenerator kpg = KeyPairGenerator.getInstance("DSA");
        SecureRandom random = SecureRandom.getInstanceStrong();
        kpg.initialize(keySize, random);
        this.keyPair = kpg.generateKeyPair();
        logger.info("DSA KeyPair generated (keySize=" + keySize + ")");
    }

    public String getPublicKeyBase64() {
        if (keyPair == null) return null;
        byte[] encoded = keyPair.getPublic().getEncoded();
        return Base64.getEncoder().encodeToString(encoded);
    }

    public String getPrivateKeyBase64() {
        if (keyPair == null) return null;
        byte[] encoded = keyPair.getPrivate().getEncoded();
        return Base64.getEncoder().encodeToString(encoded);
    }

    public String signMessage(String message) {
        try {
            Signature signer = Signature.getInstance("SHA256withDSA");
            signer.initSign(keyPair.getPrivate());
            signer.update(message.getBytes(StandardCharsets.UTF_8));
            byte[] sigBytes = signer.sign();
            return Base64.getEncoder().encodeToString(sigBytes);
        } catch (Exception e) {
            logger.log(Level.SEVERE, "Signing failed", e);
            return null;
        }
    }

    public boolean verifySignature(String message, String base64Signature) {
        try {
            Signature verifier = Signature.getInstance("SHA256withDSA");
            verifier.initVerify(keyPair.getPublic());
            verifier.update(message.getBytes(StandardCharsets.UTF_8));
            byte[] sigBytes = Base64.getDecoder().decode(base64Signature);
            return verifier.verify(sigBytes);
        } catch (Exception e) {
            logger.log(Level.WARNING, "Verification failed", e);
            return false;
        }
    }

    public PublicKey importPublicKeyFromBase64(String base64PublicKey) throws GeneralSecurityException {
        byte[] keyBytes = Base64.getDecoder().decode(base64PublicKey);
        X509EncodedKeySpec spec = new X509EncodedKeySpec(keyBytes);
        KeyFactory kf = KeyFactory.getInstance("DSA");
        return kf.generatePublic(spec);
    }

    public boolean verifyWithPublicKey(String base64PublicKey, String message, String base64Signature) {
        try {
            PublicKey pub = importPublicKeyFromBase64(base64PublicKey);
            Signature verifier = Signature.getInstance("SHA256withDSA");
            verifier.initVerify(pub);
            verifier.update(message.getBytes(StandardCharsets.UTF_8));
            byte[] sigBytes = Base64.getDecoder().decode(base64Signature);
            return verifier.verify(sigBytes);
        } catch (Exception e) {
            logger.log(Level.WARNING, "Verification with provided public key failed", e);
            return false;
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        DSASignature demo = new DSASignature();

        System.out.println("=== DSS (DSA) Digital Signature Demo ===");
        while (true) {
            System.out.println("\nMenu:");
            System.out.println("1. Generate new DSA key pair (2048 bits)");
            System.out.println("2. Show public key (Base64)");
            System.out.println("3. Show private key (Base64) [FOR DEMO ONLY]");
            System.out.println("4. Sign a message");
            System.out.println("5. Verify a signature (using stored public key)");
            System.out.println("6. Verify signature with provided public key (Base64)");
            System.out.println("7. Demonstrate tamper detection");
            System.out.println("8. Exit");
            System.out.print("Enter choice: ");

            String line = sc.nextLine().trim();
            int choice;
            try {
                choice = Integer.parseInt(line);
            } catch (NumberFormatException nfe) {
                System.out.println("Invalid input. Enter a number 1-8.");
                continue;
            }

            try {
                switch (choice) {
                    case 1:
                        demo.generateKeys(2048);
                        System.out.println("New DSA key pair generated (2048 bits).");
                        break;
                    case 2:
                        System.out.println("Public Key (Base64, X.509):");
                        System.out.println(demo.getPublicKeyBase64());
                        break;
                    case 3:
                        System.out.println("Private Key (Base64, PKCS#8) — DEMO ONLY:");
                        System.out.println(demo.getPrivateKeyBase64());
                        break;
                    case 4:
                        System.out.print("Enter message to sign: ");
                        String msgToSign = sc.nextLine();
                        String signature = demo.signMessage(msgToSign);
                        System.out.println("\nSignature (Base64):");
                        System.out.println(signature);
                        break;
                    case 5:
                        System.out.print("Enter message to verify: ");
                        String msgToVerify = sc.nextLine();
                        System.out.print("Enter signature (Base64): ");
                        String sigInput = sc.nextLine();
                        boolean ok = demo.verifySignature(msgToVerify, sigInput);
                        System.out.println("Verification result: " + (ok ? "VALID" : "INVALID"));
                        break;
                    case 6:
                        System.out.print("Enter public key (Base64, X.509): ");
                        String pubKeyInput = sc.nextLine();
                        System.out.print("Enter message to verify: ");
                        String msgPk = sc.nextLine();
                        System.out.print("Enter signature (Base64): ");
                        String sigPk = sc.nextLine();
                        boolean ok2 = demo.verifyWithPublicKey(pubKeyInput, msgPk, sigPk);
                        System.out.println("Verification with provided public key: " + (ok2 ? "VALID" : "INVALID"));
                        break;
                    case 7:
                        System.out.print("Enter message to sign for tamper demo: ");
                        String original = sc.nextLine();
                        String sig = demo.signMessage(original);
                        System.out.println("\nOriginal signature (Base64): " + sig);

                        String tamperedMessage = original + "X";
                        boolean v1 = demo.verifySignature(original, sig);
                        boolean v2 = demo.verifySignature(tamperedMessage, sig);
                        System.out.println("\nVerification with original message: " + (v1 ? "VALID" : "INVALID"));
                        System.out.println("Verification with tampered message:  " + (v2 ? "VALID" : "INVALID"));
                        System.out.println("\nConclusion: tampering the message invalidates the signature.");
                        break;
                    case 8:
                        System.out.println("Exiting. Goodbye!");
                        sc.close();
                        return;
                    default:
                        System.out.println("Enter a valid choice (1-8).");
                }
            } catch (Exception e) {
                logger.log(Level.SEVERE, "Unexpected error in menu operation", e);
            }
        }
    }
}
