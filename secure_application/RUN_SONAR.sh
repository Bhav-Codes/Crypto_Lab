#!/bin/bash

# Quick SonarQube Scanner Script
# Library Management System - Group 4

echo "=========================================="
echo "SonarQube Scanner - Library Management"
echo "=========================================="
echo ""

# Check if SonarQube server is running
echo "Checking SonarQube server..."
if curl -s http://localhost:9000/api/system/status | grep -q "UP"; then
    echo "✅ SonarQube server is running"
else
    echo "❌ SonarQube server is not running!"
    echo ""
    echo "Please start SonarQube first:"
    echo "  cd ~/sonarqube/bin/macosx-universal-64/"
    echo "  ./sonar.sh start"
    echo ""
    exit 1
fi

# Check if sonar-scanner is installed
echo "Checking sonar-scanner installation..."
if command -v sonar-scanner &> /dev/null; then
    echo "✅ sonar-scanner is installed"
    sonar-scanner --version
else
    echo "❌ sonar-scanner is not installed!"
    echo ""
    echo "Install with: brew install sonar-scanner"
    echo ""
    exit 1
fi

echo ""
echo "Starting scan..."
echo "=========================================="
echo ""

# Run the scanner
sonar-scanner

echo ""
echo "=========================================="
echo "Scan complete!"
echo ""
echo "View results at: http://localhost:9000"
echo "Project: Library Management System - Group 4"
echo "=========================================="
