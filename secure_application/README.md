# Library Management System - Assignment 3

**Group Number:** 4  
**Application:** Library Management System

## Project Overview

This is a console-based Library Management System developed for Lab Assignment 3. The application is split into two parts:

### Part 1 (Complete)
- **Book Issue/Return**
- **Member Registration**

### Part 2 (Complete)
- **Fine Calculation** — ₹5/day overdue fines with payment tracking
- **Search Operations** — ISBN search and advanced multi-filter search

## Intentional Vulnerabilities

This application contains **intentional security vulnerabilities** for SAST analysis:

1. **SQL Injection**
   - Location: `register_member()`, `display_member_info()`, `search_book()`, `calculate_fine()`, `view_all_fines()`, `pay_fine()`, `search_by_isbn()`, `advanced_search()`
   - User input is directly concatenated into SQL queries without parameterization
   - Can be exploited to manipulate database queries

2. **Cross-Site Scripting (XSS)**
   - Location: `display_member_info()`
   - User-supplied data is displayed without sanitization
   - In a web context, this could execute malicious scripts

3. **Missing Authentication**
   - Location: `issue_book()`, `return_book()`, `pay_fine()`
   - No verification of user identity or authorization
   - Anyone can issue, return books, or record fine payments without authentication

4. **Improper Input Validation**
   - Location: `calculate_fine()`, `pay_fine()`, and all user input handlers
   - No validation on IDs, amounts, or special characters
   - Negative payment amounts accepted in fine payment

⚠️ **WARNING:** This code is for educational purposes only. DO NOT use in production!

## Technology Stack

- **Language:** C++17
- **Database:** SQLite3
- **Build System:** CMake
- **Development Environment:** Console-based

## Project Structure

```
secure_application/
├── src/
│   └── library_management.cpp   # Main application code
├── CMakeLists.txt                 # Build configuration
├── reports/                       # Analysis reports
├── screenshots/                   # Application screenshots
├── sast/                         # SAST scan results
├── outputs/                      # Application output logs
├── testcases/                    # Test cases and exploits
└── README.md                     # This file
```

## Setup Instructions

### Prerequisites
- C++17 compiler (GCC, Clang, or MSVC)
- CMake 3.16 or higher
- SQLite3 development library

**Windows (vcpkg):**
```bash
vcpkg install sqlite3:x64-windows
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
```

**Linux (Debian/Ubuntu):**
```bash
sudo apt install build-essential cmake libsqlite3-dev
```

**macOS:**
```bash
brew install cmake sqlite
```

### Build and Run

1. Navigate to the secure_application directory:
```bash
cd secure_application
```

2. Configure and build:
```bash
cmake -B build
cmake --build build
```

3. Run the application (from `src/` so `library.db` is created in the right place):
```bash
cd src
../build/library_management      # Linux/macOS
..\build\Debug\library_management.exe   # Windows (MSVC)
```

## Usage Guide

### Running the Application

```bash
cd secure_application
cmake -B build && cmake --build build
cd src
../build/library_management
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

#### 8. Calculate Fine for Issue *(Part 2)*
- Calculate overdue fine for a specific issue ID
- Fine rate: ₹5 per day (configurable via `FINE_RATE_PER_DAY`)
- Shows days overdue and total fine amount
- **Vulnerability Test:** SQL injection via issue ID: `1 OR 1=1`

#### 9. View All Outstanding Fines *(Part 2)*
- Lists all currently issued books that are overdue
- Shows member name, book title, days overdue, and fine amount
- Displays total outstanding fines

#### 10. Search Book by ISBN *(Part 2)*
- Search for a book using its ISBN
- Displays complete book information
- **Vulnerability Test:** SQL injection: `' OR '1'='1`

#### 11. Advanced Book Search *(Part 2)*
- Search with multiple filters (title, author, availability)
- **Vulnerability Test:** SQL injection in title filter

#### 12. Pay Fine *(Part 2)*
- Record fine payments (full or partial)
- Tracks payment status: unpaid / partial / paid
- **Vulnerability Test:** Negative payment amount, SQL injection in fine ID

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

**In ISBN Search (Part 2):**
```
ISBN: ' OR '1'='1
```

**In Fine Calculation (Part 2):**
```
Issue ID: 1 OR 1=1
```

**In Fine Payment (Part 2):**
```
Fine ID: 1; DROP TABLE fines; --
Amount: -100
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

### Fines Table *(Part 2)*
```sql
CREATE TABLE fines (
    fine_id INTEGER PRIMARY KEY AUTOINCREMENT,
    issue_id INTEGER,
    member_id INTEGER,
    fine_amount REAL,
    fine_date TEXT,
    paid_amount REAL DEFAULT 0,
    status TEXT DEFAULT 'unpaid',
    FOREIGN KEY (issue_id) REFERENCES issued_books(issue_id),
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

## Contributors

- **Part 1:** Book Issue/Return, Member Registration
- **Part 2:** Fine Calculation, Enhanced Search Operations

## License

This is an educational project for Lab Assignment 3. Not for production use.
