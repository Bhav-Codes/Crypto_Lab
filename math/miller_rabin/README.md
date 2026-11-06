# Miller-Rabin Prime Generation (Assignment 8)

Generate a 512-bit **probable prime** using the Miller-Rabin primality test.

## Run
```bash
python3 src/miller_rabin.py
```
No external libraries (uses Python's `random` and built-in big integers).

## Sample result
| Parameter | Value |
|---|---|
| Assigned bit length | 512 |
| Actual bit length | 512 |
| Candidates tested | 13 |
| Miller-Rabin rounds | 40 |
| Execution time | ~0.02 s |

Verification: passes Miller-Rabin **and** an independent Fermat test.
(The exact prime/counts change each run — it is random, not hard-coded.)

## Concept

- **Why primes?** RSA and Diffie-Hellman need large primes. Numbers this big
  (512+ bits) can't be factored to check primality, so we use a fast
  **probabilistic** test.
- **Miller-Rabin idea:** write `n-1 = d · 2^r`. For a random base `a`, a prime
  must satisfy `a^d ≡ 1` or `a^(d·2^i) ≡ -1 (mod n)`. If neither holds, `a` is a
  "witness" proving `n` is composite. Each round with a fresh base cuts the
  chance of a wrong "prime" answer to ≤ 1/4, so after 40 rounds the error is
  astronomically small (≤ 4^-40).
- **Generation loop:** pick a random odd number with the top bit set (so it has
  exactly 512 bits), test it, repeat until one passes.
