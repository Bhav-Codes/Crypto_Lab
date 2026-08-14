# Quick Start Guide - Library Management System (C++)

## Build and Run

```bash
cd secure_application
cmake -B build
cmake --build build
cd src
../build/library_management
```

**Windows (PowerShell):**
```powershell
cd secure_application
cmake -B build
cmake --build build --config Release
cd src
..\build\Release\library_management.exe
```

## Menu Options

| Option | Feature |
|--------|---------|
| 1 | Register New Member |
| 2 | View Member Information |
| 3 | List Available Books |
| 4 | Search Books (title/author) |
| 5 | Issue Book |
| 6 | Return Book |
| 7 | View Issued Books |
| 8 | **Calculate Fine for Issue** *(Part 2)* |
| 9 | **View All Outstanding Fines** *(Part 2)* |
| 10 | **Search Book by ISBN** *(Part 2)* |
| 11 | **Advanced Book Search** *(Part 2)* |
| 12 | **Pay Fine** *(Part 2)* |
| 13 | Exit |

## Quick Test Flow (Part 2)

### Test Fine Calculation

1. Build and run the application
2. Select **9** (View All Outstanding Fines) — sample data includes an overdue issue
3. Select **8** (Calculate Fine for Issue) and enter issue ID `1`
4. Expected: 10 days overdue × Rs. 5 = **Rs. 50 fine**

### Test ISBN Search

1. Select **10** (Search Book by ISBN)
2. Enter: `978-0-7432-7356-5`
3. Expected: Details for *The Great Gatsby*

### Test Advanced Search

1. Select **11** (Advanced Book Search)
2. Title: `1984`, Author: *(leave blank)*, Available: `yes`
3. Expected: Matching available books

### Test Fine Payment

1. Run **8** first to create a fine record
2. Select **12** (Pay Fine)
3. Enter fine ID `1` and amount `25`
4. Expected: Partial payment recorded (status: partial)

## Sample Data

The app auto-creates:

- 5 sample books with ISBNs
- 1 sample member (John Doe, ID: 1)
- 1 overdue issued book (Issue ID: 1, 10 days overdue)

Delete `src/library.db` and restart to reset sample data.

## Fine Rate

Default: **Rs. 5 per day** (configurable via `LibraryManagement::FINE_RATE_PER_DAY`)

## Vulnerability Testing (Lab Only)

See `testcases/test_vulnerabilities.md` for SQL injection and input validation test cases.

## Dependencies

- C++17 compiler
- CMake 3.16+
- SQLite3 dev library (`libsqlite3-dev` on Ubuntu, `sqlite` via Homebrew on macOS)
