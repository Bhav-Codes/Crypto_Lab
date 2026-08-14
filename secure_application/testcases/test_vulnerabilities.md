# Vulnerability Test Cases

## Test Case 1: SQL Injection in Member Registration

### Objective
Demonstrate SQL injection vulnerability in the `register_member()` function.

### Steps
1. Run the application: `./library_management` (from `src/` after building)
2. Select option 1 (Register New Member)
3. Enter the following inputs:

```
Name: admin'); DROP TABLE books; --
Email: test@test.com
Phone: 1234567890
Address: Test Address
```

### Expected Result
- SQL injection attempt will be executed
- May cause database errors or unexpected behavior
- SAST tool should flag this as SQL injection vulnerability

### Severity
**HIGH** - Can lead to data loss or unauthorized database access

---

## Test Case 2: SQL Injection in Member Information Display

### Objective
Demonstrate SQL injection in member lookup functionality.

### Steps
1. Run the application
2. Select option 2 (View Member Information)
3. Enter member ID as: `1 OR 1=1`

### Expected Result
- Query may return multiple members or all members
- Bypasses intended access control
- SAST tool should detect SQL injection

### Severity
**HIGH** - Unauthorized data access

---

## Test Case 3: SQL Injection in Book Search

### Objective
Exploit SQL injection in book search functionality.

### Steps
1. Run the application
2. Select option 4 (Search Books)
3. Enter search term: `' OR '1'='1`

### Expected Result
- Returns all books regardless of search criteria
- Demonstrates SQL injection vulnerability
- SAST scanner should flag this

### Severity
**HIGH** - Can expose entire database

---

## Test Case 4: Cross-Site Scripting (XSS)

### Objective
Demonstrate XSS vulnerability in member name display.

### Steps
1. Run the application
2. Select option 1 (Register New Member)
3. Enter the following:

```
Name: <script>alert('XSS Vulnerability')</script>
Email: xss@test.com
Phone: 9876543210
Address: XSS Test
```

4. Then select option 2 and view this member's information

### Expected Result
- Name is stored and displayed without sanitization
- In a web context, this would execute JavaScript
- SAST tool should detect XSS vulnerability

### Severity
**MEDIUM** - Can lead to session hijacking in web applications

---

## Test Case 5: Missing Authentication - Unauthorized Book Issue

### Objective
Issue a book without proper authentication or authorization.

### Steps
1. Run the application
2. Select option 5 (Issue Book)
3. Enter any valid book ID and member ID
4. Book is issued without verifying user identity

### Expected Result
- Book is issued without authentication
- No verification of user permissions
- Anyone can issue books to anyone
- SAST tool should flag missing authentication

### Severity
**HIGH** - Critical authorization bypass

---

## Test Case 6: Missing Authentication - Unauthorized Book Return

### Objective
Return a book without verification of ownership.

### Steps
1. First issue a book (option 5)
2. Note the issue ID
3. Select option 6 (Return Book)
4. Enter the issue ID
5. Book is returned without verification

### Expected Result
- Book can be returned by anyone, not just the member who borrowed it
- No authentication or ownership verification
- SAST tool should detect authorization flaw

### Severity
**HIGH** - Authorization bypass

---

## Test Case 7: Input Validation Bypass

### Objective
Test improper input validation in various fields.

### Steps
1. Try registering a member with empty fields
2. Try negative member IDs
3. Try SQL special characters in multiple fields

### Expected Result
- Application may crash or behave unexpectedly
- No proper input validation
- SAST tool should flag input validation issues

### Severity
**MEDIUM** - Can lead to application crashes

---

## Test Case 8: Fine Calculation with Overdue Book

### Objective
Verify fine calculation for overdue books and demonstrate SQL injection in `calculate_fine()`.

### Steps
1. Run the application: `./library_management` (from `src/` after building)
2. Select option 9 (View All Outstanding Fines) to see overdue issues
3. Select option 8 (Calculate Fine for Issue)
4. Enter issue ID: `1`

### Expected Result
- Displays days overdue and fine amount (₹5 × days overdue)
- Fine record created in `fines` table
- Sample data: Issue ID 1 should show ~10 days overdue = ₹50 fine

### Severity
**INFO** - Functional test

---

## Test Case 9: Fine Calculation with Non-Overdue Book

### Objective
Verify no fine is charged for books not yet overdue.

### Steps
1. Issue a new book (option 5) with a valid book ID and member ID
2. Note the new issue ID from option 7 (View Issued Books)
3. Select option 8 (Calculate Fine for Issue)
4. Enter the new issue ID

### Expected Result
- Message: "Book is not overdue"
- Fine Amount: ₹0
- Days Overdue: 0

### Severity
**INFO** - Functional test

---

## Test Case 10: SQL Injection in ISBN Search

### Objective
Exploit SQL injection in `search_by_isbn()` function.

### Steps
1. Run the application
2. Select option 10 (Search Book by ISBN)
3. Enter ISBN: `' OR '1'='1`

### Expected Result
- May return unexpected book records or cause SQL errors
- Demonstrates SQL injection vulnerability in ISBN lookup
- SAST scanner should flag string concatenation in query

### Severity
**HIGH** - Can expose entire books database

---

## Test Case 11: SQL Injection in Fine Calculation

### Objective
Exploit SQL injection in `calculate_fine()` via issue ID.

### Steps
1. Run the application
2. Select option 8 (Calculate Fine for Issue)
3. Enter issue ID: `1 OR 1=1`

### Expected Result
- Query may return wrong issue record or multiple records
- Bypasses intended single-issue lookup
- SAST tool should detect SQL injection

### Severity
**HIGH** - Unauthorized data access

---

## Test Case 12: Input Validation Bypass in Fine Payment

### Objective
Demonstrate improper input validation in `pay_fine()`.

### Steps
1. First run option 8 with issue ID `1` to create a fine record
2. Select option 12 (Pay Fine)
3. Enter fine ID: `1`
4. Enter payment amount: `-100`

### Expected Result
- Negative payment accepted without validation
- Paid amount may decrease incorrectly
- No authentication required to record payment
- SAST tool should flag input validation and missing auth issues

### Severity
**MEDIUM** - Financial data manipulation

---

## Test Case 13: SQL Injection in Advanced Search

### Objective
Exploit SQL injection in `advanced_search()` title filter.

### Steps
1. Run the application
2. Select option 11 (Advanced Book Search)
3. Title: `' OR '1'='1`
4. Author: *(leave blank)*
5. Available: *(leave blank)*

### Expected Result
- Returns all books regardless of title filter
- SAST scanner should flag SQL injection

### Severity
**HIGH** - Database exposure

---

## SAST Tool Expected Findings

When you run SonarQube or any SAST tool on this code, it should detect:

### Critical Issues
1. **SQL Injection** (CWE-89)
   - Lines: register_member(), display_member_info(), search_book(), calculate_fine(), view_all_fines(), pay_fine(), search_by_isbn(), advanced_search()
   - String concatenation in SQL queries

2. **Missing Authentication** (CWE-306)
   - Lines: issue_book(), return_book(), pay_fine()
   - No user verification before critical operations

### High Issues
3. **Improper Input Validation** (CWE-20)
   - All user input functions, especially calculate_fine() and pay_fine()
   - No sanitization or validation; negative amounts accepted

### Medium Issues
4. **Cross-Site Scripting** (CWE-79)
   - display_member_info()
   - Unsanitized output

5. **Information Exposure** (CWE-200)
   - Error messages may reveal database structure

---

## Remediation Examples

### Fix for SQL Injection
```python
# BEFORE (Vulnerable)
query = f"INSERT INTO members (name, email, phone, address, registration_date) VALUES ('{name}', '{email}', '{phone}', '{address}', '{registration_date}')"
cursor.execute(query)

# AFTER (Secure)
query = "INSERT INTO members (name, email, phone, address, registration_date) VALUES (?, ?, ?, ?, ?)"
cursor.execute(query, (name, email, phone, address, registration_date))
```

### Fix for XSS
```python
# BEFORE (Vulnerable)
print(f"Name: {member[1]}")

# AFTER (Secure)
import html
print(f"Name: {html.escape(member[1])}")
```

### Fix for Missing Authentication
```python
# Add authentication layer
def authenticate_user(self, username, password):
    """Verify user credentials"""
    # Implement proper authentication
    pass

def issue_book(self, book_id, member_id, user_session):
    """Issue book with authentication check"""
    if not user_session or not user_session.is_authenticated():
        raise AuthenticationError("User not authenticated")
    
    if not self.has_permission(user_session.user_id, 'issue_book'):
        raise AuthorizationError("User not authorized")
    
    # Proceed with book issue
    pass
```

---

## Notes for Team Member

When implementing Part 2 (Fine Calculation and Search), please maintain similar vulnerability patterns:

1. **SQL Injection in Fine Calculation:**
   - Use string concatenation in fine queries

2. **Input Validation Issues:**
   - Don't validate fine amounts
   - Allow negative values

3. **Directory Traversal (Optional):**
   - If implementing report generation, allow path traversal

This ensures consistent SAST findings across both parts of the application.
