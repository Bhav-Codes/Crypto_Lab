# Vigenère Cipher Cryptanalysis

## Overview
<cite index="2-1,2-2,2-3">Implementation of Vigenère cipher cryptanalysis using Kasiski examination and frequency analysis</cite>. The program can estimate key length, perform frequency analysis, recover the encryption key, and verify results.

## Assignment Requirements

<cite index="2-2,2-3,2-4,2-6,2-8,2-10,2-12">The program implements all required functionality:
1. Preprocess the ciphertext
2. Estimate key length using Kasiski's test
3. Divide ciphertext into groups according to estimated key length
4. Perform frequency analysis on each group
5. Determine the probable key
6. Decrypt the ciphertext using recovered key
7. Display estimated key length, frequency tables, recovered key, and plaintext
8. Verify by re-encrypting recovered plaintext</cite>

## Required Functions

<cite index="2-14">All 13 user-defined functions are implemented</cite>:

| Function | Purpose | Implementation |
|----------|---------|----------------|
| `clean_ciphertext()` | Remove spaces/special characters and normalize | `vigenere_cipher.cpp` |
| `find_repeated_patterns()` | Identify repeated sequences in ciphertext | `kasiski_analysis.cpp` |
| `calculate_distances()` | Find distances between repeated occurrences | `kasiski_analysis.cpp` |
| `find_factors()` | Find factors of distances from repeated patterns | `kasiski_analysis.cpp` |
| `kasiski_analysis()` | Use patterns/distances to suggest key lengths | `kasiski_analysis.cpp` |
| `calculate_ic()` | Calculate Index of Coincidence | `kasiski_analysis.cpp` |
| `split_into_groups()` | Divide ciphertext by candidate key length | `frequency_analyzer.cpp` |
| `frequency_analysis()` | Calculate A-Z frequency for each group | `frequency_analyzer.cpp` |
| `find_shift()` | Estimate Caesar shift for a group | `frequency_analyzer.cpp` |
| `find_key()` | Combine shifts to obtain Vigenère key | `frequency_analyzer.cpp` |
| `vigenere_decrypt()` | Decrypt ciphertext using recovered key | `vigenere_cipher.cpp` |
| `vigenere_encrypt()` | Re-encrypt plaintext for verification | `vigenere_cipher.cpp` |
| `verify()` | Check if re-encryption produces original ciphertext | `main.cpp` |

## Project Structure

```
vigenere_attack/
├── src/
│   ├── kasiski_analysis.h/cpp     # Kasiski examination & IC
│   ├── frequency_analyzer.h/cpp   # Frequency analysis & key recovery
│   ├── vigenere_cipher.h/cpp      # Encryption/decryption
│   ├── main.cpp                   # Main program
│   └── Makefile                   # Build configuration
├── testcases/
│   ├── ciphertext1_odd.txt        # Test case for odd groups
│   └── ciphertext2_even.txt       # Test case for even groups
├── outputs/                       # Generated results
└── README.md                      # This file
```

## Compilation and Usage

### Build
```bash
cd src
make clean
make
```

### Run
```bash
# Interactive mode
./vigenere_cryptanalysis

# Analyze specific file
./vigenere_cryptanalysis ../testcases/ciphertext1_odd.txt

# Quick tests
make test1  # Test ciphertext 1 (odd groups)
make test2  # Test ciphertext 2 (even groups)
```

## Cryptanalysis Process

### Phase 1: Kasiski Examination
The Kasiski examination identifies repeated sequences in the ciphertext and calculates distances between repetitions. These distances are likely to be multiples of the key length.

**Algorithm**:
1. Find all repeated patterns (3-5 characters)
2. Record positions of each pattern
3. Calculate distances between occurrences
4. Factor the distances
5. Most common factors are candidate key lengths

### Phase 2: Index of Coincidence (IC)
The Index of Coincidence measures how closely letter frequencies match English text (IC ≈ 0.0667) versus random text (IC ≈ 0.0385).

**Algorithm**:
1. For each candidate key length k:
   - Split ciphertext into k groups
   - Calculate IC for each group
   - Average the IC values
2. Key length with IC closest to 0.0667 is most likely

**Formula**: IC = Σ(ni(ni-1)) / (N(N-1))

### Phase 3: Frequency Analysis
Each group represents characters encrypted with the same Caesar shift. Perform frequency analysis on each group.

**Algorithm**:
1. Split ciphertext by key length
2. For each group:
   - Count letter frequencies
   - Calculate chi-square test against shifted English frequencies
   - Find shift with minimum chi-square
3. Combine shifts to form the key

### Phase 4: Verification
Re-encrypt the recovered plaintext with the recovered key and compare with original ciphertext.

## Example Output

```
========== KASISKI EXAMINATION ==========

Top repeated patterns:
Pattern    Frequency   Distances      Positions
---------------------------------------------------
WXA        3           120,240        15,135,375
...

Key Length    Frequency (votes)
----------------------------------------
5             18
10            15
15            12
...

========== INDEX OF COINCIDENCE ANALYSIS ==========
Key Length    Avg IC          Verdict
--------------------------------------------------
5             0.0665          <-- Likely!
10            0.0412          <-- Possible
...

========== KEY RECOVERY ==========
Group    Size    Best Shift    Key Letter    Chi-Square
----------------------------------------------------------
0        120     2             C             145.23
1        120     17            R             152.34
2        120     24            Y             148.91
3        120     15            P             143.56
4        120     19            T             147.89

Recovered Key: CRYPT

========== VERIFICATION ==========
Accuracy: 100.00%
✓ SUCCESS! Verification passed!
```

## Technical Details

### Kasiski Examination Complexity
- **Time**: O(n²×m) where n = ciphertext length, m = max pattern length
- **Space**: O(p) where p = number of unique patterns

### IC Calculation Complexity
- **Time**: O(k×n) where k = max key length to test, n = ciphertext length
- **Space**: O(k×26) for frequency tables

### Frequency Analysis Complexity
- **Time**: O(k×n×26) where k = key length, n = ciphertext length
- **Space**: O(k×26) for frequency tables

## Algorithm Pseudocode

### Kasiski Examination
```
Algorithm: KASISKI_ANALYSIS(ciphertext)
1. patterns ← FIND_REPEATED_PATTERNS(ciphertext, minLen=3, maxLen=5)
2. allDistances ← []
3. FOR each pattern IN patterns DO:
4.    distances ← CALCULATE_DISTANCES(pattern.positions)
5.    allDistances ← allDistances + distances
6. factors ← FIND_FACTORS(allDistances)
7. SORT factors by frequency
8. RETURN top factors as candidate key lengths
```

### Index of Coincidence
```
Algorithm: CALCULATE_IC(text)
1. freq[26] ← COUNT_FREQUENCIES(text)
2. N ← total letters
3. ic ← 0
4. FOR i = 0 TO 25 DO:
5.    ic ← ic + (freq[i] × (freq[i] - 1))
6. ic ← ic / (N × (N - 1))
7. RETURN ic
```

### Key Recovery
```
Algorithm: FIND_KEY(ciphertext, keyLength)
1. groups ← SPLIT_INTO_GROUPS(ciphertext, keyLength)
2. key ← ""
3. FOR each group DO:
4.    shift ← FIND_SHIFT(group)  // Using chi-square
5.    key ← key + ('A' + shift)
6. RETURN key
```

### Find Shift (Chi-Square)
```
Algorithm: FIND_SHIFT(groupText)
1. observed ← FREQUENCY_ANALYSIS(groupText)
2. bestShift ← 0
3. minChiSquare ← ∞
4. FOR shift = 0 TO 25 DO:
5.    chiSquare ← 0
6.    FOR i = 0 TO 25 DO:
7.       expected ← englishFreq[i]
8.       obs ← observed[(i + shift) % 26]
9.       chiSquare ← chiSquare + ((obs - expected)² / expected)
10.   IF chiSquare < minChiSquare THEN:
11.      minChiSquare ← chiSquare
12.      bestShift ← shift
13. RETURN bestShift
```

## Ciphertext Data

<cite index="2-15,2-16">Ciphertext 1 is provided for odd group numbers</cite>.
<cite index="2-16">Ciphertext 2 is provided for even group numbers</cite>.

Both ciphertexts are included in the `testcases/` directory.

## Output Files

- `outputs/recovered_key.txt` - The recovered encryption key
- `outputs/recovered_plaintext.txt` - Decrypted plaintext
- `outputs/cleaned_ciphertext.txt` - Preprocessed ciphertext
- `outputs/analysis_report.txt` - Complete analysis report

## Tips for Analysis

1. **Pattern Length**: Longer repeated patterns (4-5 chars) are more significant
2. **Multiple Candidates**: Try top 3-5 key length candidates
3. **IC Threshold**: IC > 0.060 indicates likely correct key length
4. **Chi-Square**: Lower chi-square values indicate better fit
5. **Manual Refinement**: If verification fails, try adjusting individual key letters

## Common Issues

### Low IC Values
- Ciphertext may be too short
- Key may be very long
- Try increasing max key length parameter

### Verification Failure
- Check alternative key length candidates
- Review frequency analysis for each group
- Consider manual adjustment of key letters

### No Repeated Patterns
- Increase max pattern length
- Reduce min pattern length to 2
- Ciphertext may be too short for reliable analysis

## References

- Kasiski, Friedrich. "Die Geheimschriften und die Dechiffrir-kunst" (1863)
- Friedman, William F. "The Index of Coincidence and Its Applications in Cryptology" (1922)
- Classical cryptanalysis techniques
- English language letter frequency statistics

## Language
C++ (full implementation without Python)

## Author
Cryptography Lab Assignment - Vigenère Cipher Cryptanalysis
