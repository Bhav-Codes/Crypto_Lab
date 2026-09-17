# Quick Start Guide - Monoalphabetic Cipher Cryptanalysis

## Setup

1. **Navigate to source directory**:
   ```bash
   cd attacks/monoalphabetic_attack/src
   ```

2. **Compile the program**:
   ```bash
   make clean
   make
   ```

3. **Run the program**:
   ```bash
   ./monoalphabetic_analysis
   ```

## Usage Workflow

### Option 1: Test with Sample Data (Recommended for First Try)

1. Select option **3** from main menu
2. Program will:
   - Generate ciphertext from sample plaintext
   - Display the substitution key used
   - Start interactive cryptanalysis

### Option 2: Generate Your Own Ciphertext

1. **Prepare plaintext**:
   - Select text from "Modern Cryptography" by Katz and Lindell
   - Page number: Your_Group_Number + 30
   - Save to a file (e.g., `testcases/my_plaintext.txt`)

2. **Generate ciphertext**:
   - Select option **1** from main menu
   - Enter plaintext filename
   - Enter output ciphertext filename (e.g., `outputs/my_ciphertext.txt`)
   - Enter key filename (e.g., `outputs/my_key.txt`)

3. **Perform cryptanalysis**:
   - Select option **2** from main menu
   - Enter ciphertext filename
   - Enter plaintext filename (for verification)

## Interactive Cryptanalysis

Once in cryptanalysis mode, you'll see automated analysis results:

### Phase 1: Review Analysis
- **Letter Frequency**: Shows most common letters in ciphertext
- **Word Frequency**: Shows common 1, 2, 3-letter words
- **Pattern Analysis**: Shows repeated letter patterns

### Phase 2: Make Substitutions

**Menu Options**:
1. **View partial plaintext** - See current decryption progress
2. **View current mapping** - See all substitutions made
3. **Add substitution** - Add a cipher→plain letter mapping
4. **Remove substitution** - Remove incorrect mapping
5. **Clear all** - Start fresh
6. **Re-run analyses** - Refresh frequency/word/pattern analysis
9. **Verify solution** - Check accuracy against original
10. **Save progress** - Save current state to file
11. **Auto-suggest** - Get basic hints

### Phase 3: Iterative Process

**Example Session**:
```
Step 1: Letter 'Q' appears most (12.5%)
        → Try Q → E (E is most common in English)
        → Add substitution: Q → E
        → View partial plaintext

Step 2: Single letter word 'K' found
        → Try K → A (A is common article)
        → Add substitution: K → A
        → View partial plaintext

Step 3: Three-letter word 'XNQ' appears often
        → Notice Q=E from step 1
        → Pattern suggests "THE"
        → Add substitution: X → T, N → H
        → View partial plaintext
        → Many words now visible!

Continue until complete...
```

## Tips for Success

### Starting Strategy
1. **Start with frequency**: Map highest frequency cipher letters to E, T, A, O, I
2. **Use single letters**: Find isolated single letters (must be A or I)
3. **Look for "THE"**: Most common 3-letter word in English
4. **Common endings**: -ING, -TION, -ED

### Common Patterns
- **Double letters**: LL, EE, OO, SS are common
- **Q followed by U**: Almost always in English
- **Vowel distribution**: Usually alternates with consonants

### Validation
After each substitution:
- View partial plaintext
- Check if words make sense
- Look for conflicts
- Don't hesitate to backtrack if wrong

## Documentation for Notebook

For each substitution, document in your notebook:

| Step | Observation | Possible Substitution | Tested | Result | Decision |
|------|-------------|----------------------|---------|---------|----------|
| 1 | Q most frequent (12.5%) | Q → E | Q→E | Many valid words appeared | Keep |
| 2 | Single letter 'K' | K → A | K→A | Forms article 'A' | Keep |
| ... | ... | ... | ... | ... | ... |

## Common Issues

### Conflict Error
```
Warning: X is already mapped to Y
```
**Solution**: Review your mappings. Either:
- Remove the conflicting substitution (option 4)
- Choose different letter

### Wrong Track
If partial plaintext looks random after several steps:
- Clear all substitutions (option 5)
- Re-examine frequency analysis
- Try different hypothesis

### Verification Failed
If accuracy < 100%:
- View current mapping (option 2)
- Compare with partial plaintext
- Find mismatched letters
- Correct individual substitutions

## Output Files

- `outputs/[name]_ciphertext.txt` - Generated ciphertext
- `outputs/[name]_key.txt` - Substitution key used
- `outputs/cryptanalysis_progress.txt` - Current analysis state

## Example Complete Run

```bash
$ ./monoalphabetic_analysis
Choose option 3 (Test with sample)

[Program shows frequency analysis]
Most frequent: Q (12.5%), Z (9.2%), K (8.1%)...

[Program shows word analysis]
1-letter: K (15 times)
2-letter: RW (8 times), XR (7 times)
3-letter: XNQ (12 times), KFZ (8 times)

[Interactive menu appears]
Choice: 3 (Add substitution)
Cipher letter: Q
Plain letter: E

Choice: 1 (View partial plaintext)
Progress: 12.5%
Plaintext: _E__ E___E__ _E___...

[Continue until 100%]

Choice: 9 (Verify)
Accuracy: 100%
SUCCESS! Plaintext recovered!
```

## Next Steps

1. Run the program with sample data
2. Practice the cryptanalysis process
3. Use your own plaintext from the textbook
4. Document all steps in your notebook
5. Write algorithm pseudocode
6. Prepare your report

## Need Help?

- Review the **ALGORITHM_SUMMARY.md** for algorithm details
- Check **CRYPTANALYSIS_NOTEBOOK_TEMPLATE.md** for documentation format
- See **README.md** for comprehensive project documentation

Good luck with your cryptanalysis!
