"""
MT19937 Stream Cipher and Predictable Token Attack
CryptoLabX - Assignment 10

Shows why MT19937 (a statistical PRNG) must not be used for crypto:
  Part A - a toy stream cipher whose 16-bit seed is brute-forced.
  Part B - password reset tokens seeded with a Unix timestamp, predicted
           by searching a small time window.

Python's `random` module IS MT19937, so we use it as the "inbuilt" PRNG.
"""

import random
import time

# ----------------------------------------------------------------------
# Part A - MT19937 stream cipher
# ----------------------------------------------------------------------
def mt_crypt(data: bytes, seed: int) -> bytes:
    """XOR each byte with one 8-bit MT19937 output. Same function en/decrypts."""
    rng = random.Random(seed)
    return bytes(b ^ rng.getrandbits(8) for b in data)


def part_a():
    print("=== Part A: MT19937 stream cipher ===")

    # (2) sanity check: encrypt then decrypt a test message.
    msg = b"hello crypto lab"
    ct = mt_crypt(msg, 12345)
    assert mt_crypt(ct, 12345) == msg
    print("Encrypt/decrypt round-trip works:", mt_crypt(ct, 12345) == msg)

    # (3) plaintext = random prefix (5-20 printable) + 14 'A's, random 16-bit seed.
    prefix_len = random.randint(5, 20)
    prefix = bytes(random.randint(33, 126) for _ in range(prefix_len))
    plaintext = prefix + b"A" * 14
    secret_seed = random.randint(0, 65535)
    ciphertext = mt_crypt(plaintext, secret_seed)
    print(f"Secret 16-bit seed (hidden from attacker): {secret_seed}")

    # (4) brute force all 65,536 seeds; the right one yields 14 consecutive 'A's.
    recovered_seed, recovered_pt = None, None
    for seed in range(65536):
        guess = mt_crypt(ciphertext, seed)
        if b"A" * 14 in guess:
            recovered_seed, recovered_pt = seed, guess
            break

    # (5) verify.
    print(f"Recovered seed            : {recovered_seed}")
    print(f"Recovered plaintext       : {recovered_pt}")
    print(f"Seed search space         : 2^16 = 65536")
    print(f"Match original            : {recovered_seed == secret_seed}")


# ----------------------------------------------------------------------
# Part B - predictable password reset tokens
# ----------------------------------------------------------------------
def make_token(seed: int) -> bytes:
    """16-byte token = 16 consecutive 8-bit MT19937 outputs."""
    rng = random.Random(seed)
    return bytes(rng.getrandbits(8) for _ in range(16))


def part_b():
    print("\n=== Part B: predictable reset tokens ===")

    # (6-8) generate a token seeded with the current Unix timestamp.
    T = int(time.time())
    token = make_token(T)
    print(f"Generation timestamp T    : {T}")
    print(f"Token (hex)               : {token.hex()}")

    # (9-10) attacker searches +/- 300 seconds around T.
    WINDOW = 300
    tested = 0
    found = None
    for candidate in range(T - WINDOW, T + WINDOW + 1):
        tested += 1
        if make_token(candidate) == token:
            found = candidate
            break

    # (11) report and verify.
    print(f"Search interval           : [T-{WINDOW}, T+{WINDOW}]")
    print(f"Timestamps tested         : {tested}")
    print(f"Recovered timestamp       : {found}")
    print(f"Regenerated token matches : {make_token(found) == token}")


if __name__ == "__main__":
    part_a()
    part_b()
