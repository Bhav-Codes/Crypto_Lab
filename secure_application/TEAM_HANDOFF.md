# Team Handoff - Part 2 Implementation Guide

## Status

| Part | Features | Status |
|------|----------|--------|
| Part 1 | Book Issue/Return, Member Registration | ✅ Complete |
| Part 2 | Fine Calculation, Enhanced Search | ✅ Complete |

## Part 2 Implementation Summary

### Design Decisions

1. **Fine calculation:** Computed dynamically from `due_date` vs current date. No maximum cap applied.
2. **Fine storage:** `fines` table stores records when `calculate_fine()` is called; supports payment tracking.
3. **Fine rate:** ₹5/day via `LibraryManagement.FINE_RATE_PER_DAY` class constant.
4. **Book return:** Fines are not auto-collected on return; use menu options 8–9 to calculate/view fines first.
5. **Partial payments:** Supported via `pay_fine()` with status `unpaid` / `partial` / `paid`.

### New Database Table

```sql
CREATE TABLE fines (
    fine_id INTEGER PRIMARY KEY AUTOINCREMENT,
    issue_id INTEGER,
    member_id INTEGER,
    fine_amount REAL,
    fine_date TEXT,
    paid_amount REAL DEFAULT 0,
    status TEXT DEFAULT 'unpaid'
);
```

### New Methods

| Method | Purpose | Vulnerability |
|--------|---------|---------------|
| `calculate_fine(issue_id)` | Calculate fine for one issue | SQL Injection, no input validation |
| `view_all_fines()` | List all overdue fines | SQL Injection in date filter |
| `pay_fine(fine_id, amount)` | Record payment | SQL Injection, negative amounts allowed, no auth |
| `search_by_isbn(isbn)` | Search by ISBN | SQL Injection |
| `advanced_search(title, author, available_only)` | Multi-filter search | SQL Injection |

### Sample Test Data

On first run (empty database):

- Member: John Doe (ID 1)
- Overdue issue: Book ID 1, due 10 days ago (Issue ID 1)

Delete `library.db` and restart to reset sample data.

## Integration Points

- Extends `LibraryManagement` class in `src/library_management.cpp`
- Menu options 8–12 added; Exit moved to option 13
- Part 1 functionality unchanged

## Files Modified

- `src/library_management.cpp` — Part 2 methods and menu
- `README.md` — Part 2 documentation
- `QUICK_START.md` — Quick reference
- `testcases/test_vulnerabilities.md` — Part 2 test cases

## For SAST Analysis

Part 2 maintains the same vulnerability patterns as Part 1:

- String concatenation in SQL (not parameterized queries)
- No input validation on IDs, amounts, or ISBN
- No authentication on fine payment operations
