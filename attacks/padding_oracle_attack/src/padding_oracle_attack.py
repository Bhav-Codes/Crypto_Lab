from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes

BLOCK = 16  # AES block size in bytes

SECRET_KEY = get_random_bytes(16)       # 128-bit key, unknown to attacker
IV = get_random_bytes(16)               # random IV, known to attacker


def pkcs7_pad(data: bytes) -> bytes:
    """Add PKCS#7 padding so length is a multiple of BLOCK."""
    pad_len = BLOCK - (len(data) % BLOCK)
    return data + bytes([pad_len]) * pad_len


def pkcs7_unpad(data: bytes) -> bytes:
    """Remove PKCS#7 padding; raise ValueError if padding is invalid."""
    pad_len = data[-1]
    if pad_len < 1 or pad_len > BLOCK:
        raise ValueError("bad padding")
    if data[-pad_len:] != bytes([pad_len]) * pad_len:
        raise ValueError("bad padding")
    return data[:-pad_len]


def encrypt(plaintext: bytes) -> bytes:
    cipher = AES.new(SECRET_KEY, AES.MODE_CBC, IV)
    return cipher.encrypt(pkcs7_pad(plaintext))


oracle_queries = 0  # global counter of oracle calls


def padding_oracle(iv: bytes, ciphertext: bytes) -> bool:
    """
    The vulnerable oracle. Decrypts with the secret key and returns
    True iff the PKCS#7 padding is valid. This is the ONLY feedback
    the attacker gets.
    """
    global oracle_queries
    oracle_queries += 1
    cipher = AES.new(SECRET_KEY, AES.MODE_CBC, iv)
    decrypted = cipher.decrypt(ciphertext)
    try:
        pkcs7_unpad(decrypted)
        return True
    except ValueError:
        return False


# ----------------------------------------------------------------------
# ATTACKER SIDE. Only uses IV, the ciphertext, and padding_oracle().
# ----------------------------------------------------------------------
def recover_block(prev_block: bytes, target_block: bytes) -> bytes:
    """
    Recover the 16 plaintext bytes of one block.

    Idea: plaintext P = D(target) XOR prev_block, where D = AES block
    decryption. We cannot compute D, but we can forge a fake previous
    block C'. The oracle's decrypted byte is  D(target) XOR C'.
    By tuning C' until the padding is valid we learn D(target) byte by
    byte (right to left), then XOR with the REAL prev_block to get P.
    """
    inter = bytearray(BLOCK)   # intermediate state D(target_block)
    plain = bytearray(BLOCK)

    # Work from the last byte (index 15) to the first (index 0).
    for pos in range(BLOCK - 1, -1, -1):
        pad_val = BLOCK - pos          # padding value we are forging (1,2,3,...)
        forged = bytearray(BLOCK)

        # Set already-known tail bytes so they decrypt to pad_val.
        for i in range(pos + 1, BLOCK):
            forged[i] = inter[i] ^ pad_val

        # Brute force the current byte (0..255) until padding is valid.
        for guess in range(256):
            forged[pos] = guess
            if padding_oracle(bytes(forged), target_block):
                # Guard against false positives on the very last byte:
                # confirm by also changing the second-to-last byte.
                if pos == BLOCK - 1:
                    tweak = bytearray(forged)
                    tweak[pos - 1] ^= 0xFF
                    if not padding_oracle(bytes(tweak), target_block):
                        continue
                inter[pos] = guess ^ pad_val          # D(target)[pos]
                plain[pos] = inter[pos] ^ prev_block[pos]  # real plaintext
                break
    return bytes(plain)


def padding_oracle_attack(iv: bytes, ciphertext: bytes) -> bytes:
    """Recover the full plaintext, one block at a time."""
    blocks = [iv] + [ciphertext[i:i + BLOCK]
                     for i in range(0, len(ciphertext), BLOCK)]
    recovered = b""
    for b in range(1, len(blocks)):
        recovered += recover_block(blocks[b - 1], blocks[b])
    return pkcs7_unpad(recovered)


if __name__ == "__main__":
    message = b"Padding oracle attacks break AES-CBC without the key!"
    ciphertext = encrypt(message)

    print("[*] Ciphertext length :", len(ciphertext), "bytes",
          "(%d blocks)" % (len(ciphertext) // BLOCK))
    print("[*] Starting attack (key is never accessed)...\n")

    recovered = padding_oracle_attack(IV, ciphertext)

    print("[+] Recovered plaintext:", recovered.decode())
    print("[+] Oracle queries used:", oracle_queries)
    print("[+] Correct             :", recovered == message)
