# Library Management System - Assignment 3

**Group Number:** 4  
**Application:** Library Management System

## Project Overview

This is a console-based Library Management System developed for Lab Assignment 3. The application is split into two parts:

### Part 1 (Current Implementation)
- **Book Issue/Return**
- **Member Registration**

### Part 2 (To be completed by team member)
- **Fine Calculation**
- **Search Operations** (enhanced)

## Intentional Vulnerabilities

This application contains **3 intentional security vulnerabilities** for SAST analysis:

1. **SQL Injection**
   - Location: `register_member()`, `display_member_info()`, `search_book()`
   - User input is directly concatenated into SQL queries without parameterization
   - Can be exploited to manipulate database queries

2. **Cross-Site Scripting (XSS)**
   - Location: `display_member_info()`
   - User-supplied data is displayed without sanitization
   - In a web context, this could execute malicious scripts

3. **Missing Authentication**
   - Location: `issue_book()`, `return_book()`
   - No verification of user identity or authorization
   - Anyone can issue or return books without authentication

⚠️ **WARNING:** This code is for educational purposes only. DO NOT use in production!

## Technology Stack

- **Language:** Python 3
- **Database:** SQLite3
- **Development Environment:** Console-based

## Project Structure

```
secure_application/
├── src/
│   └── library_management.py    # Main application code
├── reports/                       # Analysis reports
├── screenshots/                   # Application screenshots
├── sast/                         # SAST scan results
├── outputs/                      # Application output logs
├── testcases/                    # Test cases and exploits
└── README.md                     # This file
```

## Setup Instructions

### Prerequisites
- Python 3.7 or higher
- SQLite3 (usually included with Python)

### Installation

1. Navigate to the secure_application directory:
```bash
cd secure_application/src
```

2. Run the application:
```bash
python3 library_management.py
```

## Usage Guide

### Running the Application

```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application/src
python3 library_management.py
```

### Features

#### 1. Register New Member
- Add new library members with name, email, phone, and address
- Automatic member ID generation
- **Vulnerability Test:** Try SQL injection in name field: `'); DROP TABLE members; --`

#### 2. View Member Information
- Display detailed member information by ID
- **Vulnerability Test:** SQL injection via member_id: `1 OR 1=1`

#### 3. List Available Books
- View all books currently available for issue
- Shows book ID, title, author, and available copies

#### 4. Search Books
- Search for books by title or author name
- **Vulnerability Test:** SQL injection: `' OR '1'='1`

#### 5. Issue Book
- Issue a book to a registered member
- Automatic due date calculation (14 days)
- **Vulnerability Test:** Issue without proper authentication

#### 6. Return Book
- Return an issued book using issue ID
- Updates available copies automatically
- **Vulnerability Test:** Return any book without verification

#### 7. View Issued Books
- View all currently issued books
- Filter by member ID (optional)

## Testing Vulnerabilities

### SQL Injection Examples

**In Member Registration:**
```
Name: admin'); DROP TABLE books; --
Email: test@test.com
Phone: 1234567890
Address: Test Address
```

**In Book Search:**
```
Search: ' OR '1'='1
```

**In Member Info:**
```
Member ID: 1 OR 1=1
```

### XSS Examples

**In Member Registration (simulated for web context):**
```
Name: <script>alert('XSS')</script>
Email: test@test.com
```

### Missing Authentication

1. Issue a book without logging in as that member
2. Return any book using just the issue ID
3. No session management or user verification

## Sample Data

The application comes with pre-populated sample books:
- The Great Gatsby by F. Scott Fitzgerald
- To Kill a Mockingbird by Harper Lee
- 1984 by George Orwell
- Pride and Prejudice by Jane Austen
- The Catcher in the Rye by J.D. Salinger

## Database Schema

### Members Table
```sql
CREATE TABLE members (
    member_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    email TEXT NOT NULL,
    phone TEXT,
    address TEXT,
    registration_date TEXT
)
```

### Books Table
```sql
CREATE TABLE books (
    book_id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    author TEXT NOT NULL,
    isbn TEXT,
    total_copies INTEGER,
    available_copies INTEGER
)
```

### Issued Books Table
```sql
CREATE TABLE issued_books (
    issue_id INTEGER PRIMARY KEY AUTOINCREMENT,
    book_id INTEGER,
    member_id INTEGER,
    issue_date TEXT,
    due_date TEXT,
    return_date TEXT,
    status TEXT,
    FOREIGN KEY (book_id) REFERENCES books(book_id),
    FOREIGN KEY (member_id) REFERENCES members(member_id)
)
```

## Running SAST Analysis

### Using SonarQube

1. Navigate to secure_application directory:
```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX/secure_application
```

2. Create sonar-project.properties:
```properties
sonar.projectKey=library-management-system
sonar.projectName=Library Management System
sonar.projectVersion=1.0
sonar.sources=src
sonar.python.version=3
```

3. Run SonarQube scanner:
```bash
sonar-scanner
```

4. View results at: http://localhost:9000

## For Team Member (Part 2)

### Getting Started

1. Pull the latest changes:
```bash
cd /Users/bhav/Stuff/crypto_lab/CryptoLabX_GroupXX
git pull origin main
```

2. Navigate to the project:
```bash
cd secure_application/src
```

3. The database file `library.db` will be created automatically when you run the application

### What to Implement

**Fine Calculation Module:**
- Calculate fines for overdue books
- Fine rate: ₹5 per day (or configure as needed)
- Display fine amount for each member
- Payment tracking

**Enhanced Search Operations:**
- Search by ISBN
- Advanced filters (by genre, publication year, etc.)
- Search history
- Recommended books feature

### Suggested Function Names

```python
def calculate_fine(self, issue_id):
    """Calculate fine for overdue book"""
    pass

def view_member_fines(self, member_id):
    """View all fines for a member"""
    pass

def pay_fine(self, fine_id, amount):
    """Record fine payment"""
    pass

def advanced_search(self, filters):
    """Enhanced search with multiple filters"""
    pass
```

### Integration Points

- Use the same `LibraryManagement` class
- Database is already initialized with all required tables
- You may need to add a `fines` table for tracking penalties
- Add your menu options starting from option 9 onwards

### Maintaining Vulnerabilities

Ensure your code also includes vulnerabilities for SAST detection:
- Continue the SQL injection pattern in new queries
- Add improper input validation in fine calculation
- Include directory traversal vulnerability if implementing file operations

## Contributors

- **Part 1:** Book Issue/Return, Member Registration
- **Part 2:** Fine Calculation, Search Operations (to be completed)

## License

This is an educational project for Lab Assignment 3. Not for production use.
