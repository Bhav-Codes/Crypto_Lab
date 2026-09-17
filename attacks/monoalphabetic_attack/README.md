# Monoalphabetic Substitution Cipher - Cryptanalysis

## Overview
This project implements a monoalphabetic substitution cipher and provides tools for cryptanalysis using frequency analysis, word patterns, and iterative plaintext recovery.

## Assignment Requirements
- Implement monoalphabetic substitution cipher
- Generate ciphertext from plaintext (1+ page from "Modern Cryptography" by Katz and Lindell)
- Perform letter-frequency analysis
- Use word patterns and repeated words
- Iteratively recover plaintext without library functions
- Recover and validate the substitution key
- Document cryptanalytic decisions

## Project Structure
```
monoalphabetic_attack/
├── src/
│   ├── monoalphabetic_cipher.h       # Cipher class definition
│   ├── monoalphabetic_cipher.cpp     # Cipher implementation
│   ├── cryptanalysis.h               # Analysis engine definition
│   ├── cryptanalysis.cpp             # Analysis implementation
│   ├── main.cpp                      # Interactive program
│   └── Makefile                      # Build configuration
├── outputs/                          # Generated ciphertexts and reports
├── testcases/                        # Sample plaintexts
├── reports/                          # Analysis documentation
└── README.md                         # This file
```

## Implementation Details

### Core Functions (Required)

#### 1. frequency_analysis()
- Counts frequency of each ciphertext letter
- Displays letters in descending frequency order
- Calculates percentage frequency
- Compares with expected English letter frequencies
- Identifies most frequent letters

#### 2. word_frequency_analysis()
- Analyzes one-letter words (A, I)
- Analyzes two-letter words (OF, TO, IN, etc.)
- Analyzes three-letter words (THE, AND, FOR, etc.)
- Identifies repeated words
- Displays word frequencies

#### 3. pattern_analysis()
- Extracts repeated letter patterns
- Identifies words with same pattern structure
- Example: HELLO has pattern 12334
- Helps match against common English words

#### 4. apply_substitution()
- Proposes and tests candidate substitutions
- Maps cipher letters to plaintext letters
- Checks for conflicts with existing mappings
- Updates partial plaintext display

#### 5. display_partial_plaintext()
- Shows current state of decryption
- Displays solved and unsolved letters
- Shows progress percentage
- Helps evaluate substitution decisions

#### 6. verify_solution()
- Validates recovered key
- Re-encrypts plaintext with recovered key
- Compares with original ciphertext
- Calculates accuracy percentage

## Compilation and Usage

### Build the Program
```bash
cd src
make clean
make
```

### Run the Program
```bash
./monoalphabetic_analysis
```

### Menu Options
1. **Generate ciphertext**: Encrypt a plaintext file with random key
2. **Perform cryptanalysis**: Interactive cryptanalysis session
3. **Test with sample**: Use built-in sample data

## Cryptanalysis Process

### Step 1: Automated Analysis
The program automatically runs:
- Letter frequency analysis
- Word frequency analysis
- Pattern analysis

### Step 2: Interactive Decryption
1. Review analysis results
2. Make substitution hypotheses based on:
   - Most frequent letters (likely E, T, A, O, I, N, S)
   - Single letter words (A or I)
   - Common two-letter words (OF, TO, IN, IT, IS)
   - Common three-letter words (THE, AND, FOR)
   - Repeated letter patterns

3. Apply substitutions iteratively
4. Examine partial plaintext
5. Reject incorrect hypotheses
6. Continue until plaintext is recovered

### Step 3: Documentation
Document each decision in your notebook using this format:

| Step | Observation | Possible Substitution | Substitution Tested | Result | Decision |
|------|-------------|----------------------|---------------------|---------|----------|
| 1 | Q occurs most frequently (12.5%) | Q → E | Q→E | Many meaningful words appeared | Good decision |
| 2 | Single letter word "K" found | K → A or K → I | K→A | Forms "A" which is common | Good decision |
| 3 | "XNQ" appears frequently | XNQ → THE | X→T, N→H, Q→E | Forms many valid words | Good decision |

## Algorithm Overview

### Encryption Algorithm
```
1. Generate or load substitution key (26 unique letters)
2. For each character in plaintext:
   a. If letter: substitute with key[letter - 'A']
   b. If not letter: keep unchanged
3. Output ciphertext
```

### Frequency Analysis Algorithm
```
1. Initialize frequency counter for A-Z
2. For each character in ciphertext:
   a. If letter: increment counter[letter]
3. Calculate percentages
4. Sort by frequency (descending)
5. Compare with English letter frequencies
```

### Pattern Analysis Algorithm
```
1. Extract all words from ciphertext
2. For each word:
   a. Generate pattern (e.g., BOOK → 1223)
   b. Store pattern with word
3. Identify repeated patterns
4. Match patterns with common English words
```

### Substitution Application Algorithm
```
1. Check for conflicts with existing mappings
2. If no conflict:
   a. Add mapping: cipher[c] = plaintext[p]
   b. Update reverse mapping: plain[p] = cipher[c]
   c. Apply to entire partial plaintext
3. Display updated partial plaintext
```

## English Language Statistics

### Most Common Letters
- E: 12.70%
- T: 9.06%
- A: 8.17%
- O: 7.51%
- I: 6.97%
- N: 6.75%

### Most Common Words
- 1-letter: A, I
- 2-letter: OF, TO, IN, IT, IS, BE, AS, AT, SO, WE
- 3-letter: THE, AND, FOR, ARE, BUT, NOT, YOU, ALL

## Example Session

```
1. Load ciphertext file
2. Run frequency analysis → Most frequent letter is 'Q'
3. Hypothesis: Q → E (E is most common in English)
4. Apply substitution Q → E
5. View partial plaintext → Many 'E's appear in reasonable positions
6. Continue with next frequent letters
7. Use word patterns to confirm/reject hypotheses
8. Iterate until complete plaintext is recovered
9. Verify solution against original plaintext
```

## Tips for Cryptanalysis

1. **Start with frequency**: Map most frequent cipher letters to E, T, A, O, I
2. **Look for single letters**: Must be A or I
3. **Use short words**: Two and three letter words are very constraining
4. **Check patterns**: Double letters (LL, EE, OO, SS) are common
5. **Validate continuously**: After each substitution, check if words make sense
6. **Be flexible**: Sometimes initial hypotheses are wrong - be ready to backtrack
7. **Use context**: Partially decoded words give clues about remaining letters

## Common Pitfalls

- Assuming first substitution is always correct
- Ignoring conflicts between hypotheses
- Not considering word boundaries
- Forgetting that Q is almost always followed by U
- Not tracking which letters are already assigned

## Output Files

- `outputs/sample_ciphertext.txt`: Generated ciphertext
- `outputs/sample_key.txt`: Substitution key used
- `outputs/cryptanalysis_progress.txt`: Current state of analysis

## References

- "Introduction to Modern Cryptography" by Katz and Lindell
- Classical cryptanalysis techniques
- English language letter and word frequencies

## Author
Group XX - Cryptography Lab Assignment

## Language
C++ (as required - Python excluded per assignment specifications)
