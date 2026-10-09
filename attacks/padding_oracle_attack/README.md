# Padding Oracle Attack (AES-CBC, PKCS#7)

Assignment 7 — recover the plaintext of an AES-CBC ciphertext **without the key**,
using only a padding oracle.

## Run
```bash
python3 src/padding_oracle_attack.py
```
Dependency: `pycryptodome` (used only as the standard AES library, which the
assignment permits). Install with `pip3 install pycryptodome`.

## Sample result
```
Recovered plaintext: Padding oracle attacks break AES-CBC without the key!
Oracle queries used: 8547
Correct             : True
```
(See `outputs/sample_run.txt`. Key/IV are random each run, so the exact query
count varies; it averages ~128 queries per byte.)

---

## 1. Understand the attack

- **AES-CBC**: AES encrypts 16-byte blocks. In CBC mode each plaintext block is
  XORed with the previous ciphertext block before encryption; the first block
  uses the IV. On decryption: `P_i = AES_decrypt(C_i) XOR C_{i-1}` (with
  `C_0 = IV`). So the previous ciphertext block directly controls the plaintext
  of the current block.
- **PKCS#7 padding**: the message is padded to a multiple of 16 bytes. If `n`
  bytes of padding are needed, each padded byte has value `n` (e.g. `03 03 03`).
  Valid padding is checked on decryption.
- **Padding oracle**: any system that reveals whether decryption produced valid
  padding (an error, a different response, or timing) acts as an oracle
  returning one bit: "padding valid?" That single bit is enough to leak the
  plaintext.

## 2. Implement the attack (what the code does)

Let `I = AES_decrypt(C_target)` (the "intermediate" block the attacker cannot
compute directly). The real plaintext is `P = I XOR C_prev`.

We replace the previous block with a forged block `C'`. The oracle now decrypts
byte `j` to `I[j] XOR C'[j]`. For each target byte, from **right to left**:

1. Pick the padding value we are forging (`1`, then `2`, ...).
2. Set the already-known tail bytes so they decrypt to that padding value.
3. Brute-force the current byte `0..255` until the oracle says padding is valid.
   Then `I[j] = guess XOR pad_val`, and `P[j] = I[j] XOR C_prev[j]`
   (XOR with the **real** previous block).

The last byte of a block gets a confirmation check (also flip the second-to-last
byte) to avoid a false `01` match. Blocks are processed independently; IV is the
"previous block" for block 1.

## 3. Analyze the attack

- **Queries**: worst case `256` per byte, average `128`, so about
  `128 × message_length` oracle calls. The sample run (64-byte ciphertext = 4
  blocks = 64 bytes) used ~8500 queries, matching `~128 × 64 ≈ 8192`.
- **Why modifying the previous ciphertext block works**: because
  `P_i = AES_decrypt(C_i) XOR C_{i-1}`, the plaintext is a direct XOR with the
  previous block. Changing `C_{i-1}` changes the decrypted plaintext of block
  `i` bit-for-bit, while `AES_decrypt(C_i)` stays fixed. This lets us steer the
  decrypted bytes to form valid padding and solve for the unknown
  `AES_decrypt(C_i)` one byte at a time — no key needed.

## 4. Security recommendation

- **Authenticated encryption**: use AES-GCM or encrypt-then-MAC. Verify the
  MAC/tag *before* decrypting or checking padding, so tampered ciphertexts are
  rejected without ever revealing padding status.
- **Do not leak the padding result**: return identical error messages and
  responses for padding errors vs. other errors, and keep decryption time
  constant to avoid a timing oracle.
