# CryptoLabX - A Cryptanalysis Toolkit

## Project Overview
CryptoLabX is a comprehensive cryptanalysis toolkit developed as part of the Cryptography Laboratory (22CPP307) course. This project aims to build a reusable software framework that will gradually evolve into a complete cryptanalysis platform throughout the semester.

## Team Members
- Bhavya Singhal
- Vanshika Bhushan
- Group: 4

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
├── utils/              # Utility modules (Coming Soon)
├── main.cpp            # Main application (single file)
└── README.md           # This file
```

## Future Modules



## Installation

1. Clone the repository:
```bash
git clone <repository-url>
cd CryptoLabX_GroupXX
```

2. Compile the project:
```bash
g++ main.cpp -o cryptolabx
```

Or with any C++ compiler:
```bash
g++ -std=c++11 main.cpp -o cryptolabx
clang++ main.cpp -o cryptolabx
```

## Usage

Run the compiled program:
```bash
./cryptolabx
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
- C++ compiler (g++, clang++, or MSVC)
- Standard C++ libraries only (no external dependencies)

## Development Guidelines
- Follow C++ best practices and coding standards
- Use header guards in all header files
- Write clear comments and documentation
- Maintain modular and reusable code
- Test thoroughly before committing
- Document all significant changes
- Use proper memory management
- Handle exceptions appropriately

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
