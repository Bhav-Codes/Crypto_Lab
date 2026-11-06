# MT19937 Stream Cipher & Predictable Token Attack (Assignment 10)

Shows why **MT19937** (a statistical PRNG) is unsafe for cryptography.
Python's `random` module *is* MT19937, so we use it as the "inbuilt" PRNG.

## Run
```bash
python3 src/mt19937_attack.py
```

## Part A — toy stream cipher + seed recovery
- **Cipher:** keystream byte = `Random(seed).getrandbits(8)`, XORed with each
  data byte. The same function encrypts and decrypts (XOR is its own inverse).
- **Attack:** plaintext is a random prefix + 14 `'A'` characters, encrypted with
  a secret 16-bit seed. The attacker tries all **2^16 = 65,536** seeds; the
  correct one is the one whose decryption contains the 14 consecutive `A`s.
- **Result:** recovered seed matches the original and decrypts the whole message.

## Part B — predictable password-reset tokens
- A 16-byte token is generated as 16 MT19937 outputs seeded with the current
  **Unix timestamp**.
- The attacker, knowing only that the token was made within ±300 seconds,
  tries each timestamp as a seed (601 candidates) and regenerates the token
  until it matches — recovering the exact generation time.

## Notebook answers

1. **MT19937 & why it's not secure:** MT19937 is a fast PRNG with a 19937-bit
   state and period 2^19937−1, built for simulations. It is **not** crypto-secure
   because its output is a reversible (tempering) function of its state: observe
   624 consecutive 32-bit outputs and you can reconstruct the full state and
   predict all future output. It is also often seeded with low-entropy values.
2. **Known plaintext reveals keystream:** for a stream cipher
   `C = P XOR K`, so `K = P XOR C`. Knowing plaintext bytes (the 14 `A`s)
   directly exposes those keystream bytes. **16-bit seed search space = 2^16 =
   65,536** — trivially brute-forced.
3. **Timestamp seeding is weak:** seconds-granularity timestamps have very low
   entropy. If the attacker roughly knows *when* a token was issued, the seed
   space collapses to a few hundred values.
4. **Pseudocode (seed recovery):**
   ```
   for seed in 0..65535:
       pt = decrypt(ciphertext, seed)
       if known_plaintext in pt: return seed
   ```
   **Pseudocode (token prediction):**
   ```
   for t in (T-300)..(T+300):
       if token(t) == observed_token: return t
   ```

**Fix:** use a CSPRNG (`secrets`, `os.urandom`, `/dev/urandom`) for keys and
tokens — never `random`/MT19937, and never seed with a timestamp.
