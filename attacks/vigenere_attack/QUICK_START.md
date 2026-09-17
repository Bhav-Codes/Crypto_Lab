# Quick Start - Vigenère Cipher Cryptanalysis

## Setup

1. **Navigate to source directory**:
   ```bash
   cd attacks/vigenere_attack/src
   ```

2. **Compile**:
   ```bash
   make clean
   make
   ```

3. **Run**:
   ```bash
   ./vigenere_cryptanalysis
   ```

## Usage Examples

### Test with Provided Ciphertexts

**For Odd Group Numbers:**
```bash
make test1
# or
./vigenere_cryptanalysis ../testcases/ciphertext1_odd.txt
```

**For Even Group Numbers:**
```bash
make test2
# or
./vigenere_cryptanalysis ../testcases/ciphertext2_even.txt
```

### Interactive Mode

```bash
./vigenere_cryptanalysis

# Then select:
# 1 - Analyze from custom file
# 2 - Analyze Ciphertext 1 (odd groups)
# 3 - Analyze Ciphertext 2 (even groups)
# 4 - Enter ciphertext manually
```

## What the Program Does

### Step 1: Preprocessing
- Removes spaces and special characters
- Converts to uppercase
- Shows cleaned ciphertext

### Step 2: Kasiski Examination
- Finds repeated patterns (3-5 characters)
- Calculates distances between repetitions
- Factors the distances
- Suggests candidate key lengths

### Step 3: Index of Coincidence
- Tests each candidate key length
- Splits ciphertext into groups
- Calculates IC for each group
- IC ≈ 0.0667 indicates likely key length

### Step 4: Combined Analysis
- Merges candidates from both methods
- Ranks by probability

### Step 5: Frequency Analysis
- For each candidate key length:
  - Splits ciphertext into groups
  - Performs frequency analysis on each group
  - Uses chi-square test to find Caesar shift
  - Combines shifts into full key

### Step 6: Key Recovery & Decryption
- Shows recovered key
- Decrypts ciphertext
- Displays plaintext

### Step 7: Verification
- Re-encrypts plaintext with recovered key
- Compares with original ciphertext
- Reports accuracy

## Sample Output

```
======================================================================
  VIGENÈRE CIPHER CRYPTANALYSIS
  Using Kasiski Examination and Frequency Analysis
======================================================================

[Step 1] Preprocessing ciphertext...
Cleaned length: 520 characters

[Step 2] Performing Kasiski Examination...
Top repeated patterns:
Pattern    Frequency   Distances      Positions
---------------------------------------------------
WXA        3           120,240        15,135,375
...

Key Length    Frequency (votes)
----------------------------------------
5             18  <-- Most votes
10            15
15            12

[Step 3] Calculating Index of Coincidence...
Key Length    Avg IC          Verdict
--------------------------------------------------
5             0.0665          <-- Likely!
10            0.0412          <-- Possible

[Step 4] Combined key length candidates: 5 10 15

[Step 5] Trying each candidate key length...

--- Testing key length = 5 ---
Group    Size    Best Shift    Key Letter    Chi-Square
----------------------------------------------------------
0        104     2             C             145.23
1        104     17            R             152.34
2        104     24            Y             148.91
3        104     15            P             143.56
4        104     19            T             147.89

Recovered key: CRYPT
IC of plaintext: 0.0667
First 100 chars: THEVIGENERECIPHERISAPOLYALPHABETICSUBSTITUTIONCIPH...

======================================================================
  BEST RESULT
======================================================================

✓ Estimated Key Length: 5
✓ Recovered Key: CRYPT
✓ IC Score: 0.0667

[Step 7] Verification
Original ciphertext length: 520
Re-encrypted text length: 520
Matching characters: 520/520
Accuracy: 100.00%

✓ SUCCESS! Verification passed!
The recovered key correctly decrypts the ciphertext.

✓ Results saved to outputs/ directory
```

## Output Files

After analysis, check the `outputs/` directory:
- `recovered_key.txt` - The encryption key
- `recovered_plaintext.txt` - Decrypted text
- `cleaned_ciphertext.txt` - Preprocessed ciphertext
- `analysis_report.txt` - Full analysis report

## Understanding the Results

### Kasiski Examination
- **Repeated patterns**: Common sequences appear at intervals
- **Distances**: Gaps between repetitions
- **Factors**: Key length divides these distances evenly
- **High vote count**: Strong indicator of key length

### Index of Coincidence
- **IC ≈ 0.0667**: English plaintext (correct key length)
- **IC ≈ 0.0385**: Random distribution (wrong key length)
- **Between**: Partially correct or short text

### Chi-Square Test
- **Lower is better**: Better match with English frequencies
- **Typical range**: 100-300 for correct shifts
- **Very high (>500)**: Likely wrong shift

### Verification
- **100% accuracy**: Key is definitely correct
- **95-99%**: Key is mostly correct, minor errors
- **<95%**: Key is likely incorrect

## Troubleshooting

### No Key Found
- Ciphertext too short (need 200+ characters)
- Try adjusting pattern length parameters
- Check if text is actually Vigenère cipher

### Low Verification Accuracy
- Check alternative key lengths (shown at end)
- Try manual key adjustment
- Ciphertext may have errors

### Very Long Key
- Program limits to key length 20
- Longer keys need more ciphertext
- Consider using only top IC candidates

## Tips for Success

1. **Use long ciphertext**: 500+ characters ideal
2. **Trust IC over Kasiski**: IC is more reliable
3. **Check multiple candidates**: Top 3-5 key lengths
4. **Look at plaintext**: Even partial success shows patterns
5. **Manual refinement**: Adjust individual key letters if needed

## Advanced Usage

### Analyze Custom File
```bash
./vigenere_cryptanalysis path/to/your/ciphertext.txt
```

### Modify Parameters
Edit `kasiski_analysis.h` to change:
- `minPatternLength` (default: 3)
- `maxPatternLength` (default: 5)
- Max key length in IC (default: 20)

### Debug Mode
Add more detailed output by modifying source:
- Uncomment frequency table displays
- Add intermediate results logging

## Next Steps

1. Run analysis on provided ciphertexts
2. Document results in your notebook
3. Try with your own Vigenère ciphertexts
4. Experiment with different key lengths
5. Compare Kasiski vs IC effectiveness

For detailed algorithms and theory, see `README.md`.
