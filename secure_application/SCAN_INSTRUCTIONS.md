# Quick SonarQube Scan Instructions

## 🚀 Fastest Way to Run Scan

### Option 1: Using the Script (Easiest)

```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
./RUN_SONAR.sh
```

### Option 2: Manual Command

```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
sonar-scanner
```

Then open: **http://localhost:9000**

---

## 📋 Prerequisites Checklist

Before running the scan, make sure:

- [ ] SonarQube server is running
- [ ] sonar-scanner is installed
- [ ] You're in the correct directory

### Quick Checks:

```bash
# 1. Check if SonarQube is running
curl http://localhost:9000/api/system/status

# 2. Check if sonar-scanner is installed
sonar-scanner --version

# 3. Verify you're in the right place
pwd
# Should show: /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
```

---

## 🔧 If SonarQube Server is Not Running

```bash
# Find your SonarQube installation (usually in home directory)
cd ~/sonarqube/bin/macosx-universal-64/

# Start SonarQube
./sonar.sh start

# Wait 1-2 minutes, then check status
./sonar.sh status

# Open in browser
open http://localhost:9000
```

---

## 📦 If sonar-scanner is Not Installed

```bash
# Install using Homebrew
brew install sonar-scanner

# Verify installation
sonar-scanner --version
```

---

## 🎯 Scanning Different Directories

### Scan Only secure_application (Library Management):
```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
sonar-scanner
```

### Scan Entire CryptoLabX Project:
```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX
sonar-scanner
```

**Note:** Make sure there's a `sonar-project.properties` file in the directory you're scanning from.

---

## 📊 Viewing Results

1. **Open browser:** http://localhost:9000
2. **Login:** (if prompted)
   - Username: `admin`
   - Password: `admin` (or your password)
3. **Find your project:** "Library Management System - Group 4"
4. **Review findings:**
   - Vulnerabilities tab (SQL Injection, etc.)
   - Security Hotspots
   - Code Smells
   - Bugs

---

## 📸 Taking Screenshots for Report

Navigate to these sections and take screenshots:

1. **Dashboard Overview**
   - Shows overall project health
   - Bugs, Vulnerabilities, Code Smells count

2. **Issues → Vulnerabilities**
   - Filter by severity: Critical, High
   - Expand each SQL Injection finding

3. **Security Hotspots**
   - Review security-sensitive code locations

4. **Code Tab**
   - View specific vulnerable code sections

Save screenshots to: `secure_application/screenshots/`

---

## 🐛 Troubleshooting

### Error: "Could not find sonar-project.properties"

**Fix:**
```bash
# Make sure you're in the right directory
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application

# Verify the file exists
ls -la sonar-project.properties
```

### Error: "Invalid authentication token"

**Fix:**
1. Go to http://localhost:9000
2. Login
3. Click profile (top right) → My Account → Security
4. Generate new token
5. Update `sonar-project.properties`:
   ```
   sonar.token=YOUR_NEW_TOKEN_HERE
   ```

### Error: "Connection refused to localhost:9000"

**Fix:** Start SonarQube server
```bash
cd ~/sonarqube/bin/macosx-universal-64/
./sonar.sh start
```

### Warning: "No sources to analyze"

**Fix:** Check that src folder exists
```bash
ls -la src/
# Should show library_management.py
```

---

## ✅ Expected Scan Results

You should see findings for:

### Critical/High Severity:
- **SQL Injection** (3-5 instances)
  - In `register_member()`
  - In `search_book()`
  - In `display_member_info()`

### Medium Severity:
- **Missing Authentication** (2 instances)
  - In `issue_book()`
  - In `return_book()`
- **XSS/Output Encoding** (1-2 instances)
- **Input Validation Issues** (multiple)

---

## 📁 Where Files Are Located

```
secure_application/
├── sonar-project.properties     ← Configuration (already set up)
├── RUN_SONAR.sh                 ← Quick scan script
├── src/
│   └── library_management.py    ← Code to be scanned
├── screenshots/                 ← Save screenshots here
├── reports/                     ← Save reports here
└── sast/                        ← Save SAST results here
```

---

## 🎓 For Your Lab Report

Include:
1. Screenshot of dashboard overview
2. Screenshots of each vulnerability finding
3. Brief description of each vulnerability
4. Code snippets showing the vulnerable code
5. Explanation of why it's vulnerable
6. How it could be exploited
7. How to fix it (optional)

---

## 💡 Pro Tips

1. **Run scan after code changes** to see new findings
2. **Use project key** to track different versions
3. **Export JSON** for detailed analysis
4. **Take notes** while reviewing findings
5. **Mark false positives** if any (there shouldn't be for this assignment)

---

## 🔄 Quick Command Summary

```bash
# Full workflow
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
./RUN_SONAR.sh
open http://localhost:9000

# Or manual
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
sonar-scanner
```

---

## 📞 Need More Help?

See detailed guide: `SONARQUBE_GUIDE.md`

---

**Ready to scan?** Just run:
```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
./RUN_SONAR.sh
```

🎯 Good luck with your analysis!
