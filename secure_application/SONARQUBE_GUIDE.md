# SonarQube Scanning Guide - Library Management System

## Prerequisites

Before running SonarQube, ensure you have:

1. ✅ SonarQube Server running (http://localhost:9000)
2. ✅ SonarQube Scanner installed
3. ✅ Authentication token generated

## Step-by-Step Instructions

### Step 1: Start SonarQube Server (if not running)

```bash
# Navigate to SonarQube installation directory
cd ~/sonarqube/bin/macosx-universal-64/

# Start SonarQube
./sonar.sh start

# Check status
./sonar.sh status

# Wait for server to be ready (takes 1-2 minutes)
# Open browser: http://localhost:9000
```

**Default Login:**
- Username: `admin`
- Password: `admin` (change on first login if prompted)

### Step 2: Generate Authentication Token (if you don't have one)

1. Open http://localhost:9000
2. Login with admin credentials
3. Click on your profile (top right) → My Account
4. Go to "Security" tab
5. Generate a new token:
   - Name: `library-management-scan`
   - Type: `User Token`
   - Click "Generate"
6. **Copy the token** (you won't see it again!)

### Step 3: Update sonar-project.properties

The `sonar-project.properties` file is already created in the secure_application directory. Update it with your token:

```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
nano sonar-project.properties
```

Update this line:
```properties
sonar.token=YOUR_ACTUAL_TOKEN_HERE
```

Or use your existing token from sast_lab:
```properties
sonar.token=sqp_0a106d23d3e86b0febe8b8a95c9d0dd1e6f1e4d7
```

### Step 4: Run SonarQube Scanner

#### Method 1: Scan Only secure_application Folder

```bash
# Navigate to secure_application directory
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application

# Run scanner
sonar-scanner
```

#### Method 2: Scan from Parent Directory (Entire CryptoLabX Project)

First, create a sonar-project.properties in the parent directory:

```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX

# Create configuration
cat > sonar-project.properties << 'EOF'
# SonarQube Configuration for Entire CryptoLabX Project
# Group 4 - Lab Assignments

# Project identification
sonar.projectKey=cryptolabx-group4-complete
sonar.projectName=CryptoLabX - Group 4 Complete Project
sonar.projectVersion=1.0

# Host URL
sonar.host.url=http://localhost:9000

# Authentication token - Use your token
sonar.token=sqp_0a106d23d3e86b0febe8b8a95c9d0dd1e6f1e4d7

# Source code locations
sonar.sources=classical,modern,attacks,analysis,secure_application/src

# Python specific settings
sonar.language=py
sonar.python.version=3

# C++ settings (if needed for other parts)
# sonar.cfamily.compile-commands=compile_commands.json

# Encoding
sonar.sourceEncoding=UTF-8

# Exclusions
sonar.exclusions=**/*.md,**/*.txt,**/datasets/**,**/outputs/**,**/__pycache__/**,**/*.db,**/a.out,**/*.o
EOF

# Run scanner
sonar-scanner
```

### Step 5: Monitor Scan Progress

Watch the terminal output:

```
INFO: Scanner configuration file: /usr/local/Cellar/sonar-scanner/X.X.X/libexec/conf/sonar-scanner.properties
INFO: Project root configuration file: /path/to/sonar-project.properties
INFO: SonarScanner 5.x.x
INFO: Java 17.x.x
INFO: Analyzing on SonarQube server 10.x
...
INFO: ANALYSIS SUCCESSFUL
INFO: ------------------------------------------------------------------------
INFO: EXECUTION SUCCESS
INFO: ------------------------------------------------------------------------
```

### Step 6: View Results

1. Open your browser: http://localhost:9000
2. Click on your project:
   - `library-management-system-group4` (if scanned secure_application only)
   - `cryptolabx-group4-complete` (if scanned entire project)
3. Review the dashboard showing:
   - Bugs
   - Vulnerabilities
   - Security Hotspots
   - Code Smells
   - Coverage
   - Duplications

## Expected Findings for Library Management System

### Critical/High Severity Issues

1. **SQL Injection (CWE-89)**
   - Severity: CRITICAL
   - Locations:
     - `register_member()` - Line ~60
     - `display_member_info()` - Line ~80
     - `search_book()` - Line ~140
   - Issue: String concatenation in SQL queries

2. **Missing Authentication (CWE-306)**
   - Severity: HIGH
   - Locations:
     - `issue_book()` - Line ~170
     - `return_book()` - Line ~220
   - Issue: No authentication checks before sensitive operations

### Medium Severity Issues

3. **Cross-Site Scripting (CWE-79)**
   - Severity: MEDIUM
   - Location: `display_member_info()` - Line ~85-90
   - Issue: Unsanitized output

4. **Improper Input Validation (CWE-20)**
   - Severity: MEDIUM
   - Multiple locations
   - Issue: No validation of user inputs

### Code Smells

- Long methods
- Code duplication
- Hardcoded values
- Exception handling issues

## Troubleshooting

### Issue: "sonar-scanner: command not found"

**Solution:** Install sonar-scanner

```bash
# Using Homebrew
brew install sonar-scanner

# Verify installation
sonar-scanner --version
```

### Issue: "Could not connect to SonarQube server"

**Solution:** Ensure SonarQube is running

```bash
# Check if server is running
curl http://localhost:9000/api/system/status

# If not running, start it
cd ~/sonarqube/bin/macosx-universal-64/
./sonar.sh start
```

### Issue: "Invalid authentication token"

**Solution:** Generate a new token

1. Go to http://localhost:9000
2. My Account → Security → Generate Token
3. Update sonar-project.properties with new token

### Issue: "Project key already exists"

**Solution:** Either delete the old project or use a different project key

```bash
# Change projectKey in sonar-project.properties
sonar.projectKey=library-management-system-group4-v2
```

### Issue: "No sources to analyze"

**Solution:** Check your paths in sonar.sources

```bash
# Verify files exist
ls -la src/

# Update sonar-project.properties if needed
sonar.sources=src
```

## Advanced Options

### Generate HTML Report

```bash
# Install sonar-report (if not installed)
npm install -g sonar-report

# Generate report
sonar-report \
  --sonarurl="http://localhost:9000" \
  --sonarcomponent="library-management-system-group4" \
  --sonartoken="sqp_your_token_here" \
  --output="sast/sonarqube-report.html"
```

### Export Results to JSON

```bash
# Using curl to fetch issues
curl -u sqp_your_token_here: \
  "http://localhost:9000/api/issues/search?componentKeys=library-management-system-group4&ps=500" \
  > sast/sonarqube-issues.json
```

### Scan Specific Files Only

```bash
# Create temporary config
cat > sonar-temp.properties << EOF
sonar.projectKey=library-management-test
sonar.projectName=Library Management - Specific Files
sonar.host.url=http://localhost:9000
sonar.token=sqp_your_token_here
sonar.sources=src/library_management.py
sonar.sourceEncoding=UTF-8
EOF

# Run scan
sonar-scanner -Dproject.settings=sonar-temp.properties
```

## Scanning from Parent Directory - Quick Reference

### Option 1: Scan Only secure_application

```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
sonar-scanner
```

### Option 2: Scan Entire CryptoLabX Project

```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX

# Create parent-level config (one-time)
cat > sonar-project.properties << 'EOF'
sonar.projectKey=cryptolabx-complete
sonar.projectName=CryptoLabX - Complete Project
sonar.host.url=http://localhost:9000
sonar.token=sqp_0a106d23d3e86b0febe8b8a95c9d0dd1e6f1e4d7
sonar.sources=classical,modern,attacks,analysis,secure_application/src
sonar.sourceEncoding=UTF-8
sonar.exclusions=**/*.md,**/__pycache__/**,**/*.db
EOF

# Run scan
sonar-scanner
```

### Option 3: Multiple Separate Scans

Scan each component separately with different project keys:

```bash
# Scan secure_application
cd secure_application
sonar-scanner -Dsonar.projectKey=group4-library-management

# Scan classical crypto
cd ../classical
sonar-scanner -Dsonar.projectKey=group4-classical-crypto

# etc.
```

## Saving Results

### Take Screenshots

1. Dashboard Overview
2. Issues → Vulnerabilities tab
3. Security Hotspots
4. Each critical SQL Injection finding

Save to: `secure_application/screenshots/`

### Export PDF Report

1. Go to project dashboard
2. Click "More" → "Download PDF"
3. Save to: `secure_application/reports/`

### Save JSON Data

```bash
# Create reports directory if needed
mkdir -p secure_application/reports

# Export issues
curl -u sqp_your_token_here: \
  "http://localhost:9000/api/issues/search?componentKeys=library-management-system-group4&ps=500" \
  -o secure_application/reports/sonarqube-issues.json

# Export metrics
curl -u sqp_your_token_here: \
  "http://localhost:9000/api/measures/component?component=library-management-system-group4&metricKeys=bugs,vulnerabilities,code_smells,security_hotspots" \
  -o secure_application/reports/sonarqube-metrics.json
```

## Quick Checklist

- [ ] SonarQube server is running (http://localhost:9000)
- [ ] Authentication token is generated and added to config
- [ ] sonar-project.properties is properly configured
- [ ] sonar-scanner is installed
- [ ] Navigated to correct directory
- [ ] Run: `sonar-scanner`
- [ ] Check results at http://localhost:9000
- [ ] Take screenshots of findings
- [ ] Export reports
- [ ] Document vulnerabilities found

## Important Notes

1. **Database files:** SQLite .db files are excluded from scanning
2. **Python cache:** __pycache__ directories are excluded
3. **Documentation:** .md files are excluded
4. **Token security:** Never commit your authentication token to git

## Need Help?

Check SonarQube logs:
```bash
# Scanner logs
cat ~/.sonar/log/sonar-scanner.log

# Server logs
tail -f ~/sonarqube/logs/sonar.log
```

## Next Steps After Scanning

1. Review all findings in SonarQube dashboard
2. Take screenshots for your lab report
3. Document each vulnerability found
4. Export reports for submission
5. Update your README with scan results
6. Add findings to your lab assignment report

---

**Ready to scan?** Run:
```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
sonar-scanner
```

Then open: http://localhost:9000 🎯
