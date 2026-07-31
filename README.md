# CryptoLabX - A Cryptanalysis Toolkit

## Project Overview
CryptoLabX is a comprehensive cryptanalysis toolkit developed as part of the Cryptography Laboratory (22CPP307) course. This project aims to build a reusable software framework that will gradually evolve into a complete cryptanalysis platform throughout the semester.

## Team Members
- [Add team member names here]
- Group: XX

## Project Description
This toolkit serves as a foundation for cryptographic analysis and experimentation. It is designed with modularity and extensibility in mind, allowing for easy integration of new cryptographic algorithms, attack methods, and analysis tools in future iterations.

## Current Features (Week 1)
- **Menu-Driven Interface**: Interactive command-line interface with multiple options
- **File Analysis**: Comprehensive text file analysis including:
  - Character, word, and line count
  - Unique character identification
  - Letter frequency distribution
  - Statistical analysis for cryptanalysis preparation
- **Activity Logging**: Automatic logging of all user actions with timestamps
- **Modular Architecture**: Well-organized codebase for future expansion

## Folder Structure

```
CryptoLabX_GroupXX/
├── classical/          # Classical cipher implementations (Coming Soon)
├── attacks/            # Cryptanalysis attack methods (Coming Soon)
├── math/               # Mathematical utilities for cryptography (Coming Soon)
├── modern/             # Modern cryptographic algorithms (Coming Soon)
├── analysis/           # Analysis and statistical tools (Coming Soon)
├── datasets/           # Sample text files for testing
│   ├── sample1.txt     # Pangram for alphabet testing
│   ├── sample2.txt     # Cryptography overview
│   ├── sample3.txt     # Caesar cipher description
│   ├── sample4.txt     # Frequency analysis information
│   └── sample5.txt     # Cryptanalyst description
├── outputs/            # Generated outputs and logs
│   └── activity.log    # Activity log file (auto-generated)
├── docs/               # Documentation (Coming Soon)
├── tests/              # Unit tests (Coming Soon)
├── utils/              # Utility modules
│   ├── __init__.py
│   ├── logger.py       # Logging functionality
│   └── file_analyzer.py # File analysis tools
├── main.py             # Main application entry point
├── README.md           # This file
└── requirements.txt    # Python dependencies
```

## Future Modules

### Classical Ciphers (Week 2-4)
- Caesar Cipher
- Substitution Cipher
- Vigenère Cipher
- Playfair Cipher
- Rail Fence Cipher
- Columnar Transposition

### Modern Cryptography (Week 5-8)
- AES (Advanced Encryption Standard)
- DES (Data Encryption Standard)
- RSA (Rivest-Shamir-Adleman)
- Diffie-Hellman Key Exchange
- Hash Functions (SHA, MD5)

### Attack Methods (Week 9-12)
- Brute Force Attack
- Frequency Analysis
- Dictionary Attack
- Known Plaintext Attack
- Chosen Plaintext Attack
- Meet-in-the-Middle Attack

### Analysis Tools (Week 13-15)
- Statistical Analysis
- Entropy Calculation
- Pattern Recognition
- Avalanche Effect Testing
- Randomness Testing

### Mathematical Utilities
- Prime Number Generation
- GCD and Extended Euclidean Algorithm
- Modular Arithmetic
- Matrix Operations
- Polynomial Operations

## Installation

1. Clone the repository:
```bash
git clone <repository-url>
cd CryptoLabX_GroupXX
```

2. Install dependencies:
```bash
pip install -r requirements.txt
```

## Usage

Run the main application:
```bash
python main.py
```

### Menu Options
1. **Encrypt** - Encryption functionality (Coming Soon)
2. **Decrypt** - Decryption functionality (Coming Soon)
3. **Attack** - Cryptanalysis attacks (Coming Soon)
4. **Analyze** - File analysis tool (Currently Active)
5. **Exit** - Exit the application

### File Analysis Feature
The Analyze option allows you to:
- Select a text file from the datasets folder
- View comprehensive statistics including:
  - Total character count
  - Total word count
  - Total line count
  - Number of unique characters
  - Letter frequency distribution
  - Top 5 most frequent letters with percentages

## Requirements
- Python 3.6 or higher
- Standard Python libraries (no external dependencies required for Week 1)

## Development Guidelines
- Follow PEP 8 style guide for Python code
- Write docstrings for all functions and classes
- Maintain modular and reusable code
- Test thoroughly before committing
- Document all significant changes

## Activity Logging
All user actions are automatically logged in `outputs/activity.log` with:
- Date and time of action
- Action performed (Encrypt, Decrypt, Attack, Analyze, Exit)

## Testing
To test the file analysis feature:
1. Run the application
2. Select option 4 (Analyze)
3. Choose any sample file from the list
4. View the statistical analysis output

## Version History
- **v1.0.0** (Week 1): Project foundation with menu system, file analysis, and logging

## License
This project is developed for educational purposes as part of the Cryptography Laboratory course.

## Contact
For questions or contributions, please contact the team members or submit issues through the project repository.

---
*Last Updated: July 31, 2026*
