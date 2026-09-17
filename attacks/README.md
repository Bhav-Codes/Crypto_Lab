# Attack Methods

This directory contains implementations of various cryptanalysis attack methods on classical ciphers.

## Implemented Attacks

### 1. Shift Cipher Attack
**Directory**: `shift_cipher_attack/`

Cryptanalysis methods for breaking Caesar/Shift ciphers:
- **Brute Force Attack**: Try all 26 possible shifts
- **Dictionary-based Attack**: Match decrypted words against English dictionary
- **Chi-Square Attack**: Statistical analysis using letter frequency distribution

**Language**: Python  
**Status**: ✅ Complete

---

### 2. Monoalphabetic Substitution Cipher Attack
**Directory**: `monoalphabetic_attack/`

Comprehensive cryptanalysis of monoalphabetic substitution ciphers using:
- **Letter Frequency Analysis**: Count and analyze letter frequencies
- **Word Frequency Analysis**: Analyze 1, 2, 3-letter word patterns
- **Pattern Analysis**: Detect repeated letter patterns in words
- **Iterative Substitution**: Interactive hypothesis testing and refinement
- **Solution Verification**: Validate recovered key by re-encryption

**Language**: C++  
**Status**: ✅ Complete  
**Features**:
- All 6 required functions implemented (frequency_analysis, word_frequency_analysis, pattern_analysis, apply_substitution, display_partial_plaintext, verify_solution)
- Interactive cryptanalysis tool
- Step-by-step documentation support
- No library functions used for core analysis
- Comprehensive algorithm documentation

**Quick Start**:
```bash
cd monoalphabetic_attack/src
make
./monoalphabetic_analysis
```

---

### 3. Vigenère Cipher Attack
**Directory**: `vigenere_attack/`

Advanced cryptanalysis of polyalphabetic substitution using:
- **Kasiski Examination**: Find repeated patterns and distance analysis
- **Index of Coincidence (IC)**: Statistical method for key length estimation
- **Frequency Analysis**: Chi-square test on grouped ciphertext
- **Automated Key Recovery**: Complete key reconstruction
- **Verification**: Re-encryption validation

**Language**: C++  
**Status**: ✅ Complete  
**Features**:
- All 13 required functions implemented
- Kasiski examination with pattern detection
- IC calculation for key length estimation
- Chi-square based frequency analysis
- Automatic key recovery and decryption
- Comprehensive verification system
- Handles both provided test ciphertexts

**Quick Start**:
```bash
cd vigenere_attack/src
make
./vigenere_cryptanalysis ../testcases/ciphertext1_odd.txt
```

---

## Planned Implementations

Future attack methods to be implemented:
- Playfair Cipher Attack
- Hill Cipher Attack (Known-plaintext attack)
- Transposition Cipher Attack
- Modern cipher attacks (if applicable)

---

## Project Structure

Each attack implementation follows this structure:
```
attack_name/
├── src/              # Source code
├── outputs/          # Generated outputs
├── testcases/        # Test data
├── reports/          # Analysis reports
├── dictionary/       # Word lists (if needed)
└── README.md         # Documentation
```

---

## Usage Notes

- Each attack is self-contained in its own directory
- Detailed documentation and algorithms are provided in each subdirectory
- Sample test cases are included for quick testing
- Follow the QUICK_START.md in each directory for usage instructions

---

## Assignment Compliance

All implementations follow assignment requirements:
- ✅ No disallowed libraries used
- ✅ Custom implementations of cryptanalysis algorithms
- ✅ Proper documentation and algorithm descriptions
- ✅ Test cases and sample data included
- ✅ Step-by-step analysis capability
