# Monoalphabetic Substitution Cipher - Cryptanalysis Notebook

## Assignment Information
- **Group Number**: XX
- **Date**: [Date]
- **Plaintext Source**: Modern Cryptography by Katz and Lindell, Page [Group_number + 30]

## Encryption Phase

### Plaintext Details
- **File**: `testcases/plaintext.txt`
- **Length**: [X] characters
- **Content**: [Brief description]

### Generated Key
```
Plain:  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
Cipher: [Insert generated substitution key]
```

### Ciphertext
- **File**: `outputs/ciphertext.txt`
- **Length**: [X] characters

---

## Cryptanalysis Phase

### Initial Frequency Analysis

#### Letter Frequencies in Ciphertext
| Rank | Cipher Letter | Count | Percentage | Expected English Letter | Expected % |
|------|---------------|-------|------------|------------------------|------------|
| 1    | [Letter]      | [N]   | [X.XX]%    | E                      | 12.70%     |
| 2    | [Letter]      | [N]   | [X.XX]%    | T                      | 9.06%      |
| 3    | [Letter]      | [N]   | [X.XX]%    | A                      | 8.17%      |
| 4    | [Letter]      | [N]   | [X.XX]%    | O                      | 7.51%      |
| 5    | [Letter]      | [N]   | [X.XX]%    | I                      | 6.97%      |

### Word Frequency Analysis

#### One-Letter Words
| Word | Frequency | Likely Plaintext |
|------|-----------|------------------|
| [X]  | [N]       | A or I           |

#### Two-Letter Words
| Word | Frequency | Possible Plaintext |
|------|-----------|-------------------|
| [XX] | [N]       | OF, TO, IN, IT... |

#### Three-Letter Words
| Word | Frequency | Possible Plaintext |
|------|-----------|-------------------|
| [XXX]| [N]       | THE, AND, FOR...  |

### Pattern Analysis

#### Repeated Letter Patterns
| Pattern | Example Word | Frequency | Possible Matches |
|---------|--------------|-----------|------------------|
| 1234    | [ABCD]       | [N]       | [words]          |
| 1223    | [ABBC]       | [N]       | [words]          |

---

## Iterative Decryption Process

### Step-by-Step Substitution Decisions

| Step | Observation | Possible Substitution | Substitution Tested | Result | Decision |
|------|-------------|----------------------|---------------------|---------|----------|
| 1    | [Letter] occurs most frequently ([X]%) | [C] → E | [C]→E | Many substitutions were useful. Partial plaintext shows 'E' in reasonable positions. | Good decision - Keep |
| 2    | Single letter word '[X]' found | [X] → A | [X]→A | Forms article 'A' which appears before nouns | Good decision - Keep |
| 3    | Three-letter word '[XXX]' appears [N] times | [ABC] → THE | [A]→T, [B]→H, [C]→E | Forms 'THE' which is most common English word | Good decision - Keep |
| 4    | Two-letter word '[XX]' after 'THE' | [AB] → OF | [A]→O, [B]→F | Forms 'OF' which is common after THE | Good decision - Keep |
| 5    |             |                      |                     |         |          |
| 6    |             |                      |                     |         |          |
| 7    |             |                      |                     |         |          |
| 8    |             |                      |                     |         |          |
| 9    |             |                      |                     |         |          |
| 10   |             |                      |                     |         |          |

### Intermediate States

#### After Step 3
```
Partial Plaintext: THE ____ __ _______ ___...
Progress: 25%
Confidence: Medium
```

#### After Step 6
```
Partial Plaintext: THE ____ OF _______ ___...
Progress: 50%
Confidence: High
```

---

## Final Solution

### Recovered Substitution Key
```
Cipher: [Full 26-letter key recovered]
Plain:  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
```

### Verification Results
- **Method**: Re-encrypted recovered plaintext with recovered key
- **Accuracy**: [XX.XX]% match with original ciphertext
- **Status**: ✓ Success / ✗ Partial

### Recovered Plaintext (First 200 characters)
```
[Insert recovered plaintext]
```

---

## Analysis and Insights

### What Worked Well
1. **Frequency Analysis**: [Describe effectiveness]
2. **Pattern Matching**: [Describe effectiveness]
3. **Word Analysis**: [Describe effectiveness]

### Challenges Encountered
1. [Challenge 1 and how it was resolved]
2. [Challenge 2 and how it was resolved]

### Key Observations
- Most helpful clue: [Description]
- Most difficult substitution: [Description]
- Time to complete: [X] minutes/hours

### Lessons Learned
1. [Lesson 1]
2. [Lesson 2]
3. [Lesson 3]

---

## Algorithm Summary

### Frequency Analysis Algorithm
```
1. Initialize frequency counter for each letter (A-Z)
2. Scan through ciphertext character by character
3. For each alphabetic character:
   a. Convert to uppercase
   b. Increment counter for that letter
4. Calculate total letter count
5. For each letter:
   a. Calculate percentage = (count / total) × 100
6. Sort letters by frequency (descending)
7. Display alongside expected English frequencies
8. Identify top candidates for common letters (E, T, A, O, I, N, S)
```

### Word Frequency Analysis Algorithm
```
1. Initialize maps for 1-letter, 2-letter, and 3-letter words
2. Parse ciphertext into words (split on non-alphabetic characters)
3. For each word:
   a. Get word length
   b. If length ≤ 3:
      - Add to appropriate length category
      - Increment frequency counter
4. For each category:
   a. Sort words by frequency (descending)
   b. Display top words with their counts
5. Suggest likely plaintext candidates based on English word frequencies
```

### Pattern Analysis Algorithm
```
1. Extract all words from ciphertext
2. For each word:
   a. Generate pattern representation:
      - First unique letter → '1'
      - Second unique letter → '2'
      - Repeated letters get same number
      - Example: HELLO → 12334
   b. Store pattern with word and frequency
3. Group words by pattern
4. Identify patterns that appear multiple times
5. Match patterns with known English word patterns
6. Display repeated patterns as candidates for substitution
```

### Apply Substitution Algorithm
```
1. Input: cipher letter C, plaintext letter P
2. Check if C already mapped:
   a. If mapped to different letter: CONFLICT - reject
   b. If mapped to same letter: already done
3. Check if P already used:
   a. If used by different cipher letter: CONFLICT - reject
   b. If used by same cipher letter: already done
4. If no conflicts:
   a. Add mapping: substitution[C] = P
   b. Add reverse: reverse_substitution[P] = C
5. Update partial plaintext:
   a. Scan entire ciphertext
   b. For each occurrence of C:
      - Replace with P (preserve case)
6. Return success
```

### Display Partial Plaintext Algorithm
```
1. Initialize display with underscores for unsolved letters
2. For each character in ciphertext:
   a. If not alphabetic: keep as-is
   b. If alphabetic:
      - Check if cipher letter has substitution
      - If yes: show plaintext letter (preserve case)
      - If no: show underscore '_'
3. Calculate progress:
   a. Count total alphabetic characters
   b. Count characters with substitutions
   c. Percentage = (solved / total) × 100
4. Display ciphertext and partial plaintext side-by-side
5. Show progress percentage
```

### Verify Solution Algorithm
```
1. Input: recovered plaintext, original ciphertext, recovered key
2. Re-encrypt plaintext using recovered key:
   a. For each character in plaintext:
      - If alphabetic: substitute using key
      - Otherwise: keep unchanged
3. Compare re-encrypted text with original ciphertext:
   a. Count matching characters
   b. Calculate accuracy = (matches / total) × 100
4. If accuracy ≥ 99.5%: SUCCESS
5. Otherwise: PARTIAL - continue analysis
6. Display verification results
```

---

## Conclusion

The monoalphabetic substitution cipher was successfully [broken/partially broken] using systematic frequency and pattern analysis. The key insights were [summary]. This exercise demonstrates that monoalphabetic ciphers are vulnerable to statistical analysis when sufficient ciphertext is available.

**Final Accuracy**: [XX.XX]%  
**Completion Status**: ✓ Complete / ⚠ Partial

---

## Appendix

### Complete Frequency Tables
[Include detailed frequency data if needed]

### All Pattern Matches
[Include comprehensive pattern analysis if needed]

### Source Code Location
`/attacks/monoalphabetic_attack/src/`
