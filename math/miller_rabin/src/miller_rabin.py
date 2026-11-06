"""
Cryptographic Prime Generation using the Miller-Rabin primality test.
CryptoLabX - Assignment 8

Generates a probable prime of a given bit length (default 512), then
verifies it with an independent test (Fermat).
"""

import random
import time

BIT_LENGTH = 512     # assigned bit length
MR_ROUNDS = 40       # number of Miller-Rabin rounds (bases)


def miller_rabin(n: int, rounds: int) -> bool:
    """Return True if n is 'probably prime', False if definitely composite."""
    if n < 2:
        return False
    # Quick trial division by a few small primes.
    for p in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31):
        if n % p == 0:
            return n == p

    # Write n-1 = d * 2^r with d odd.
    r, d = 0, n - 1
    while d % 2 == 0:
        d //= 2
        r += 1

    # Test with 'rounds' random bases.
    for _ in range(rounds):
        a = random.randrange(2, n - 1)
        x = pow(a, d, n)                 # a^d mod n
        if x == 1 or x == n - 1:
            continue                     # this base says "probably prime"
        for _ in range(r - 1):
            x = pow(x, 2, n)             # square it
            if x == n - 1:
                break
        else:
            return False                 # no witness found -> composite
    return True


def fermat_test(n: int, rounds: int = 20) -> bool:
    """Independent check: a^(n-1) == 1 (mod n) for random a."""
    for _ in range(rounds):
        a = random.randrange(2, n - 1)
        if pow(a, n - 1, n) != 1:
            return False
    return True


def generate_prime(bits: int, rounds: int):
    """Generate a probable prime of exactly `bits` bits. Returns (prime, tries)."""
    tries = 0
    while True:
        tries += 1
        # Random odd number with the top and bottom bit set so it has
        # exactly `bits` bits and is odd.
        candidate = random.getrandbits(bits) | (1 << (bits - 1)) | 1
        if miller_rabin(candidate, rounds):
            return candidate, tries


if __name__ == "__main__":
    start = time.time()
    prime, tries = generate_prime(BIT_LENGTH, MR_ROUNDS)
    elapsed = time.time() - start

    print("=== Miller-Rabin Prime Generation ===\n")
    print(f"{'Assigned bit length':<28}: {BIT_LENGTH}")
    print(f"{'Generated prime candidate':<28}: {prime}")
    print(f"{'Actual bit length':<28}: {prime.bit_length()}")
    print(f"{'Number of candidates tested':<28}: {tries}")
    print(f"{'Miller-Rabin rounds':<28}: {MR_ROUNDS}")
    print(f"{'Total execution time':<28}: {elapsed:.4f} s")

    print("\n=== Verification ===")
    print(f"{'Bit length == assigned':<28}: {prime.bit_length() == BIT_LENGTH}")
    print(f"{'Passes Miller-Rabin':<28}: {miller_rabin(prime, MR_ROUNDS)}")
    print(f"{'Passes independent Fermat':<28}: {fermat_test(prime)}")
