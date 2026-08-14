# Merged Project Guide - Library Management System

## 🎉 Merge Successful!

Your changes and your teammate's Part 2 implementation have been successfully merged!

### What Changed?

**Your teammate converted the project from Python to C++!**

- ❌ Removed: `library_management.py` (Python version)
- ✅ Added: `library_management.cpp` (C++ version)
- ✅ Added: `CMakeLists.txt` (Build configuration)
- ✅ Updated: Documentation and guides

## Project Status

✅ **Part 1 Complete:** Book Issue/Return, Member Registration  
✅ **Part 2 Complete:** Fine Calculation, Enhanced Search Operations  
✅ **Both parts merged:** Ready to build and test  
✅ **Pushed to GitHub:** All team members have access  

## 📋 What's Included

### Part 1 Features (Your Work):
- Member registration
- Book issue/return
- View member information
- List available books
- Search books by title/author

### Part 2 Features (Teammate's Work):
- Calculate fines for overdue books
- View all outstanding fines
- Search books by ISBN
- Fine payment tracking
- Enhanced search operations

### Vulnerabilities (Combined):
1. **SQL Injection** - Multiple locations
2. **Cross-Site Scripting (XSS)** - In display functions
3. **Missing Authentication** - In critical operations
4. **Improper Input Validation** - In fine calculations

## 🚀 How to Build and Run

### Prerequisites

1. **C++ Compiler** (already have Clang on macOS)
2. **CMake** (install if needed)
3. **SQLite3** (usually pre-installed on macOS)

### Check Prerequisites:

```bash
# Check C++ compiler
clang++ --version

# Check CMake
cmake --version

# If CMake not installed:
brew install cmake

# Check SQLite3
sqlite3 --version
```

### Build Steps:

```bash
# Navigate to secure_application
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# Or use make directly
make
```

### Run the Application:

```bash
# From build directory
./library_management

# Or from secure_application directory
./build/library_management
```

## 📁 Current Project Structure

```
secure_application/
├── src/
│   ├── library_management.cpp      # Complete C++ implementation
│   └── __pycache__/                 # Old Python cache (can delete)
├── build/                           # Build output (create this)
│   └── library_management           # Compiled executable
├── outputs/                         # Application logs
├── reports/                         # SAST reports
├── sast/                           # SAST scan results
├── screenshots/                    # Demo screenshots
├── testcases/
│   └── test_vulnerabilities.md     # Test cases
├── CMakeLists.txt                  # Build configuration
├── README.md                       # Project documentation
├── QUICK_START.md                  # Quick start guide
├── TEAM_HANDOFF.md                 # Team collaboration docs
├── SONARQUBE_GUIDE.md              # SAST scanning guide
├── SCAN_INSTRUCTIONS.md            # Quick scan reference
├── RUN_SONAR.sh                    # Automated scan script
└── sonar-project.properties        # SAST configuration
```

## 🧪 Quick Test Workflow

### 1. Build the Project

```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
mkdir -p build && cd build
cmake ..
make
```

### 2. Run the Application

```bash
./library_management
```

### 3. Test Features

**Menu Options (now 11 total):**
1. Register New Member
2. View Member Information
3. List Available Books
4. Search Books
5. Issue Book
6. Return Book
7. View Issued Books
8. Calculate Fine for Issue *(NEW - Part 2)*
9. View All Outstanding Fines *(NEW - Part 2)*
10. Search Book by ISBN *(NEW - Part 2)*
11. Exit

### 4. Test Vulnerabilities

**SQL Injection in Member Registration:**
```
Choose option 1
Name: admin'); DROP TABLE books; --
Email: test@test.com
```

**SQL Injection in Book Search:**
```
Choose option 4
Search: ' OR '1'='1
```

**SQL Injection in ISBN Search:**
```
Choose option 10
ISBN: ' OR '1'='1
```

## 🔍 Running SAST Analysis

### Option 1: C++ with SonarQube

You'll need to update the SonarQube configuration for C++:

```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application

# Update sonar-project.properties for C++
cat > sonar-project.properties << 'EOF'
sonar.projectKey=sast-lab-group4
sonar.projectName=SAST Lab - Library Management (C++)
sonar.projectVersion=2.0
sonar.host.url=http://localhost:9000
sonar.token=sqp_0a106d23d3e86b0febe8b8a95c9d0dd1e6f1e4d7

# C++ specific settings
sonar.sources=src
sonar.language=cpp
sonar.cfamily.compile-commands=build/compile_commands.json
sonar.sourceEncoding=UTF-8

# Exclusions
sonar.exclusions=**/*.md,**/__pycache__/**,**/*.db,build/**
EOF

# Generate compile_commands.json
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

# Run SonarQube scanner
cd ..
sonar-scanner
```

### Option 2: Use Cppcheck (Alternative)

```bash
# Install cppcheck
brew install cppcheck

# Run analysis
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
cppcheck --enable=all --inconclusive --xml --xml-version=2 src/library_management.cpp 2> sast/cppcheck-results.xml

# View results
cat sast/cppcheck-results.xml
```

### Option 3: Use Clang Static Analyzer

```bash
# Run clang static analyzer
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
clang++ --analyze -Xanalyzer -analyzer-output=text src/library_management.cpp -I/usr/local/include -lsqlite3
```

## 🐛 Troubleshooting

### Build Issues

**Issue: SQLite3 not found**
```bash
# Install SQLite3 development files
brew install sqlite3

# Or use system SQLite
export SQLite3_DIR=/usr/local/opt/sqlite
```

**Issue: CMake version too old**
```bash
# Update CMake
brew upgrade cmake
```

**Issue: Compiler errors**
```bash
# Use Clang directly
clang++ -std=c++17 -o library_management src/library_management.cpp -lsqlite3
```

### Runtime Issues

**Issue: Database file not created**
```bash
# Ensure you have write permissions
cd secure_application
chmod +w .
./build/library_management
```

**Issue: SQLite3 library not found**
```bash
# Check library path
otool -L build/library_management

# Set library path if needed
export DYLD_LIBRARY_PATH=/usr/local/lib:$DYLD_LIBRARY_PATH
```

## 📊 Git Summary

### Recent Commits:

```bash
git log --oneline -5
```

Output:
- `8831a42` Merge teammate's changes - Library Management System Part 2
- `e89286f` Add SonarQube scanning guides and scripts
- `90c9794` Part 2
- `3a4a478` Add Library Management System Part 1
- `fd6268c` Lab 3 library management system init

### Changes Made:

**By You:**
- Original Python implementation (Part 1)
- SonarQube scanning guides
- Documentation and test cases

**By Teammate:**
- C++ conversion of entire project
- Part 2 implementation (Fine calculation, ISBN search)
- CMake build system
- Updated documentation

**Merged Together:**
- Complete C++ Library Management System
- Both Part 1 and Part 2 features
- All documentation and guides
- SAST analysis setup

## ✅ Next Steps

1. **Build the project:**
   ```bash
   cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
   mkdir -p build && cd build && cmake .. && make
   ```

2. **Test the application:**
   ```bash
   ./library_management
   ```

3. **Test vulnerabilities:**
   - Try SQL injection attacks
   - Test all menu options
   - Verify fine calculations

4. **Run SAST analysis:**
   - Use SonarQube (need C++ configuration)
   - Or use Cppcheck/Clang analyzer
   - Take screenshots of findings

5. **Document for report:**
   - Compile and run screenshots
   - SAST findings screenshots
   - Vulnerability demonstrations
   - Build and test process

## 🎓 For Your Lab Report

Include:
1. **Project overview** - Both parts implementation
2. **Build instructions** - CMake commands
3. **Feature demonstrations** - Screenshots of all 11 menu options
4. **Vulnerability findings** - SAST scan results
5. **Exploitation examples** - SQL injection demonstrations
6. **Team collaboration** - Git workflow and merge process

## 📞 Need Help?

- **Build issues:** Check CMakeLists.txt and compiler versions
- **Runtime issues:** Check SQLite3 installation
- **SAST issues:** See SONARQUBE_GUIDE.md
- **Git issues:** See GIT_WORKFLOW.md

---

**Status:** ✅ Merge Complete | ✅ Both Parts Integrated | ⏳ Ready to Build & Test

**Next Command:**
```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
mkdir -p build && cd build && cmake .. && make && ./library_management
```

Good luck with your assignment! 🚀
