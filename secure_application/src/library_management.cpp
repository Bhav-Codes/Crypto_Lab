/*
 * Library Management System - Parts 1 & 2 (C++)
 * Part 1: Book Issue/Return and Member Registration
 * Part 2: Fine Calculation and Enhanced Search Operations
 * Group 4 - Assignment 3
 *
 * WARNING: This application contains intentional security vulnerabilities for educational purposes.
 * Vulnerabilities included:
 * 1. SQL Injection in member registration, book search, fine calculation, and ISBN search
 * 2. Cross-Site Scripting (XSS) in display functions
 * 3. Missing Authentication checks in critical operations
 * 4. Improper Input Validation in fine calculation and payment tracking
 *
 * DO NOT use this code in production!
 */

#include <algorithm>
#include <chrono>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <sqlite3.h>

class LibraryManagement {
public:
    static const int FINE_RATE_PER_DAY = 5;  // Rs. 5 per day - configurable fine rate

    explicit LibraryManagement(const std::string& dbName = "library.db")
        : dbName_(dbName), db_(nullptr) {
        initializeDatabase();
    }

    ~LibraryManagement() {
        close();
    }

    LibraryManagement(const LibraryManagement&) = delete;
    LibraryManagement& operator=(const LibraryManagement&) = delete;

    int registerMember(const std::string& name, const std::string& email,
                       const std::string& phone, const std::string& address) {
        /*
         * VULNERABILITY: SQL Injection - User input is directly concatenated into SQL query
         */
        const std::string registrationDate = currentDate();

        // VULNERABLE: SQL Injection vulnerability
        const std::string query =
            "INSERT INTO members (name, email, phone, address, registration_date) VALUES ('" +
            name + "', '" + email + "', '" + phone + "', '" + address + "', '" +
            registrationDate + "')";

        char* errMsg = nullptr;
        if (sqlite3_exec(db_, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cout << "\n[ERROR] Registration failed: " << (errMsg ? errMsg : "unknown") << "\n";
            sqlite3_free(errMsg);
            return -1;
        }

        const int memberId = static_cast<int>(sqlite3_last_insert_rowid(db_));
        std::cout << "\n[SUCCESS] Member registered successfully!\n";
        std::cout << "Member ID: " << memberId << "\n";
        std::cout << "Name: " << name << "\n";
        std::cout << "Email: " << email << "\n";
        return memberId;
    }

    bool displayMemberInfo(const std::string& memberId) {
        /*
         * VULNERABILITY: XSS - User input displayed without sanitization
         * VULNERABILITY: SQL Injection in search
         */
        const std::string query = "SELECT * FROM members WHERE member_id = " + memberId;

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Query failed: " << sqlite3_errmsg(db_) << "\n";
            return false;
        }

        const bool found = sqlite3_step(stmt) == SQLITE_ROW;
        if (found) {
            // VULNERABLE: XSS - displaying unsanitized user input
            std::cout << "\n==================================================\n";
            std::cout << "Member ID: " << sqlite3_column_int(stmt, 0) << "\n";
            std::cout << "Name: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) << "\n";
            std::cout << "Email: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) << "\n";
            std::cout << "Phone: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) << "\n";
            std::cout << "Address: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) << "\n";
            std::cout << "Registration Date: "
                      << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) << "\n";
            std::cout << "==================================================\n";
        } else {
            std::cout << "\n[ERROR] Member not found!\n";
        }

        sqlite3_finalize(stmt);
        return found;
    }

    void listAvailableBooks() {
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db_, "SELECT * FROM books WHERE available_copies > 0", -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Query failed: " << sqlite3_errmsg(db_) << "\n";
            return;
        }

        bool hasBooks = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!hasBooks) {
                std::cout << "\n================================================================================\n";
                std::cout << std::left << std::setw(5) << "ID"
                          << std::setw(30) << "Title"
                          << std::setw(25) << "Author"
                          << std::setw(10) << "Available" << "\n";
                std::cout << "================================================================================\n";
                hasBooks = true;
            }

            std::cout << std::setw(5) << sqlite3_column_int(stmt, 0)
                      << std::setw(30) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
                      << std::setw(25) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))
                      << std::setw(10) << sqlite3_column_int(stmt, 5) << "\n";
        }

        if (hasBooks) {
            std::cout << "================================================================================\n";
        } else {
            std::cout << "\n[INFO] No books available at the moment.\n";
        }

        sqlite3_finalize(stmt);
    }

    void searchBook(const std::string& searchTerm) {
        /*
         * VULNERABILITY: SQL Injection in search query
         */
        const std::string query =
            "SELECT * FROM books WHERE title LIKE '%" + searchTerm +
            "%' OR author LIKE '%" + searchTerm + "%'";

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Search failed: " << sqlite3_errmsg(db_) << "\n";
            return;
        }

        bool hasBooks = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!hasBooks) {
                std::cout << "\n================================================================================\n";
                std::cout << "Search Results for: " << searchTerm << "\n";
                std::cout << "================================================================================\n";
                std::cout << std::left << std::setw(5) << "ID"
                          << std::setw(30) << "Title"
                          << std::setw(25) << "Author"
                          << std::setw(10) << "Available" << "\n";
                std::cout << "================================================================================\n";
                hasBooks = true;
            }

            std::cout << std::setw(5) << sqlite3_column_int(stmt, 0)
                      << std::setw(30) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
                      << std::setw(25) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))
                      << std::setw(10) << sqlite3_column_int(stmt, 5) << "\n";
        }

        if (hasBooks) {
            std::cout << "================================================================================\n";
        } else {
            std::cout << "\n[INFO] No books found matching '" << searchTerm << "'\n";
        }

        sqlite3_finalize(stmt);
    }

    bool issueBook(const std::string& bookIdStr, const std::string& memberIdStr) {
        /*
         * VULNERABILITY: Missing Authentication - No check if user is authorized
         */
        sqlite3_stmt* stmt = nullptr;

        if (sqlite3_prepare_v2(db_, "SELECT * FROM books WHERE book_id = ? AND available_copies > 0", -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Query failed: " << sqlite3_errmsg(db_) << "\n";
            return false;
        }

        sqlite3_bind_text(stmt, 1, bookIdStr.c_str(), -1, SQLITE_TRANSIENT);
        const bool bookFound = sqlite3_step(stmt) == SQLITE_ROW;
        const std::string bookTitle = bookFound ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) : "";
        const std::string bookAuthor = bookFound ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) : "";
        sqlite3_finalize(stmt);

        if (!bookFound) {
            std::cout << "\n[ERROR] Book not available or doesn't exist!\n";
            return false;
        }

        if (sqlite3_prepare_v2(db_, "SELECT * FROM members WHERE member_id = ?", -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Query failed: " << sqlite3_errmsg(db_) << "\n";
            return false;
        }

        sqlite3_bind_text(stmt, 1, memberIdStr.c_str(), -1, SQLITE_TRANSIENT);
        const bool memberFound = sqlite3_step(stmt) == SQLITE_ROW;
        const std::string memberName = memberFound ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) : "";
        sqlite3_finalize(stmt);

        if (!memberFound) {
            std::cout << "\n[ERROR] Member not found!\n";
            return false;
        }

        const std::string issueDate = currentDate();
        const std::string dueDate = dateOffsetDays(14);

        if (sqlite3_prepare_v2(
                db_,
                "INSERT INTO issued_books (book_id, member_id, issue_date, due_date, status) VALUES (?, ?, ?, ?, 'issued')",
                -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Failed to issue book: " << sqlite3_errmsg(db_) << "\n";
            return false;
        }

        sqlite3_bind_text(stmt, 1, bookIdStr.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, memberIdStr.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, issueDate.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, dueDate.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cout << "\n[ERROR] Failed to issue book: " << sqlite3_errmsg(db_) << "\n";
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);

        if (sqlite3_prepare_v2(db_, "UPDATE books SET available_copies = available_copies - 1 WHERE book_id = ?", -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Failed to update copies: " << sqlite3_errmsg(db_) << "\n";
            return false;
        }

        sqlite3_bind_text(stmt, 1, bookIdStr.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        std::cout << "\n[SUCCESS] Book issued successfully!\n";
        std::cout << "Book: " << bookTitle << " by " << bookAuthor << "\n";
        std::cout << "Member: " << memberName << "\n";
        std::cout << "Issue Date: " << issueDate << "\n";
        std::cout << "Due Date: " << dueDate << "\n";
        return true;
    }

    bool returnBook(const std::string& issueIdStr) {
        /*
         * VULNERABILITY: Missing Authentication - No verification of who is returning
         */
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(
                db_,
                "SELECT * FROM issued_books WHERE issue_id = ? AND status = 'issued'",
                -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Query failed: " << sqlite3_errmsg(db_) << "\n";
            return false;
        }

        sqlite3_bind_text(stmt, 1, issueIdStr.c_str(), -1, SQLITE_TRANSIENT);
        const bool issueFound = sqlite3_step(stmt) == SQLITE_ROW;
        const int bookId = issueFound ? sqlite3_column_int(stmt, 1) : 0;
        sqlite3_finalize(stmt);

        if (!issueFound) {
            std::cout << "\n[ERROR] Issue record not found or book already returned!\n";
            return false;
        }

        const std::string returnDate = currentDate();

        if (sqlite3_prepare_v2(
                db_,
                "UPDATE issued_books SET return_date = ?, status = 'returned' WHERE issue_id = ?",
                -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Failed to return book: " << sqlite3_errmsg(db_) << "\n";
            return false;
        }

        sqlite3_bind_text(stmt, 1, returnDate.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, issueIdStr.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cout << "\n[ERROR] Failed to return book: " << sqlite3_errmsg(db_) << "\n";
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);

        if (sqlite3_prepare_v2(db_, "UPDATE books SET available_copies = available_copies + 1 WHERE book_id = ?", -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Failed to update copies: " << sqlite3_errmsg(db_) << "\n";
            return false;
        }

        sqlite3_bind_int(stmt, 1, bookId);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        std::cout << "\n[SUCCESS] Book returned successfully!\n";
        std::cout << "Issue ID: " << issueIdStr << "\n";
        std::cout << "Return Date: " << returnDate << "\n";
        return true;
    }

    void viewIssuedBooks(const std::string& memberId = "") {
        sqlite3_stmt* stmt = nullptr;
        const char* query =
            memberId.empty()
                ? "SELECT i.issue_id, b.title, b.author, m.name, i.issue_date, i.due_date, i.status "
                  "FROM issued_books i "
                  "JOIN books b ON i.book_id = b.book_id "
                  "JOIN members m ON i.member_id = m.member_id "
                  "WHERE i.status = 'issued'"
                : "SELECT i.issue_id, b.title, b.author, m.name, i.issue_date, i.due_date, i.status "
                  "FROM issued_books i "
                  "JOIN books b ON i.book_id = b.book_id "
                  "JOIN members m ON i.member_id = m.member_id "
                  "WHERE i.member_id = ? AND i.status = 'issued'";

        if (sqlite3_prepare_v2(db_, query, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Query failed: " << sqlite3_errmsg(db_) << "\n";
            return;
        }

        if (!memberId.empty()) {
            sqlite3_bind_text(stmt, 1, memberId.c_str(), -1, SQLITE_TRANSIENT);
        }

        bool hasIssues = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!hasIssues) {
                std::cout << "\n====================================================================================================\n";
                std::cout << std::left << std::setw(10) << "Issue ID"
                          << std::setw(30) << "Book Title"
                          << std::setw(20) << "Author"
                          << std::setw(20) << "Member"
                          << std::setw(12) << "Due Date" << "\n";
                std::cout << "====================================================================================================\n";
                hasIssues = true;
            }

            std::cout << std::setw(10) << sqlite3_column_int(stmt, 0)
                      << std::setw(30) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
                      << std::setw(20) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))
                      << std::setw(20) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
                      << std::setw(12) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) << "\n";
        }

        if (hasIssues) {
            std::cout << "====================================================================================================\n";
        } else {
            std::cout << "\n[INFO] No issued books found.\n";
        }

        sqlite3_finalize(stmt);
    }

    bool calculateFine(const std::string& issueId) {
        /*
         * VULNERABILITY: SQL Injection - issue_id concatenated into query
         * VULNERABILITY: Improper Input Validation - no validation on issue_id
         */
        const std::string query =
            "SELECT i.issue_id, i.member_id, b.title, m.name, i.due_date, i.status "
            "FROM issued_books i "
            "JOIN books b ON i.book_id = b.book_id "
            "JOIN members m ON i.member_id = m.member_id "
            "WHERE i.issue_id = " + issueId;

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Fine calculation failed: " << sqlite3_errmsg(db_) << "\n";
            return false;
        }

        if (sqlite3_step(stmt) != SQLITE_ROW) {
            std::cout << "\n[ERROR] Issue record not found for ID: " << issueId << "\n";
            sqlite3_finalize(stmt);
            return false;
        }

        const int issueIdVal = sqlite3_column_int(stmt, 0);
        const int memberId = sqlite3_column_int(stmt, 1);
        const std::string bookTitle = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const std::string memberName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const std::string dueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        const std::string status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        sqlite3_finalize(stmt);

        if (status != "issued") {
            std::cout << "\n[INFO] Book has already been returned (Issue ID: " << issueIdVal << ").\n";
            std::cout << "No outstanding fine for returned books.\n";
            return true;
        }

        const int daysOverdue = computeDaysOverdue(dueDate);
        if (daysOverdue == 0) {
            std::cout << "\n[INFO] Book is not overdue.\n";
            std::cout << "Issue ID: " << issueIdVal << "\n";
            std::cout << "Book: " << bookTitle << "\n";
            std::cout << "Member: " << memberName << "\n";
            std::cout << "Due Date: " << dueDate << "\n";
            std::cout << "Days Overdue: 0\n";
            std::cout << "Fine Amount: Rs. 0\n";
            return true;
        }

        const double fineAmount = daysOverdue * FINE_RATE_PER_DAY;
        const std::string fineDate = currentDate();

        const std::string existingQuery =
            "SELECT fine_id FROM fines WHERE issue_id = " + std::to_string(issueIdVal);
        char* errMsg = nullptr;
        sqlite3_stmt* existingStmt = nullptr;

        if (sqlite3_prepare_v2(db_, existingQuery.c_str(), -1, &existingStmt, nullptr) == SQLITE_OK &&
            sqlite3_step(existingStmt) == SQLITE_ROW) {
            sqlite3_finalize(existingStmt);
            const std::string updateQuery =
                "UPDATE fines SET fine_amount = " + std::to_string(fineAmount) +
                ", fine_date = '" + fineDate + "' WHERE issue_id = " + std::to_string(issueIdVal);
            sqlite3_exec(db_, updateQuery.c_str(), nullptr, nullptr, &errMsg);
        } else {
            sqlite3_finalize(existingStmt);
            const std::string insertQuery =
                "INSERT INTO fines (issue_id, member_id, fine_amount, fine_date, paid_amount, status) VALUES (" +
                std::to_string(issueIdVal) + ", " + std::to_string(memberId) + ", " +
                std::to_string(fineAmount) + ", '" + fineDate + "', 0, 'unpaid')";
            sqlite3_exec(db_, insertQuery.c_str(), nullptr, nullptr, &errMsg);
        }

        if (errMsg) {
            std::cout << "\n[ERROR] Fine calculation failed: " << errMsg << "\n";
            sqlite3_free(errMsg);
            return false;
        }

        std::cout << "\n============================================================\n";
        std::cout << "FINE CALCULATION\n";
        std::cout << "============================================================\n";
        std::cout << "Issue ID: " << issueIdVal << "\n";
        std::cout << "Book: " << bookTitle << "\n";
        std::cout << "Member: " << memberName << "\n";
        std::cout << "Due Date: " << dueDate << "\n";
        std::cout << "Days Overdue: " << daysOverdue << "\n";
        std::cout << "Fine Rate: Rs. " << FINE_RATE_PER_DAY << "/day\n";
        std::cout << "Total Fine: Rs. " << fineAmount << "\n";
        std::cout << "============================================================\n";
        return true;
    }

    void viewAllFines() {
        /*
         * VULNERABILITY: SQL Injection - dynamic date filter in query
         */
        const std::string today = currentDate();
        const std::string query =
            "SELECT i.issue_id, b.title, m.name, i.due_date, i.member_id "
            "FROM issued_books i "
            "JOIN books b ON i.book_id = b.book_id "
            "JOIN members m ON i.member_id = m.member_id "
            "WHERE i.status = 'issued' AND i.due_date < '" + today + "' "
            "ORDER BY i.due_date";

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Failed to retrieve fines: " << sqlite3_errmsg(db_) << "\n";
            return;
        }

        bool hasFines = false;
        double totalFines = 0.0;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!hasFines) {
                std::cout << "\n====================================================================================================\n";
                std::cout << std::left << std::setw(10) << "Issue ID"
                          << std::setw(30) << "Book Title"
                          << std::setw(20) << "Member"
                          << std::setw(15) << "Days Overdue"
                          << std::setw(10) << "Fine (Rs.)" << "\n";
                std::cout << "====================================================================================================\n";
                hasFines = true;
            }

            const int issueId = sqlite3_column_int(stmt, 0);
            const std::string bookTitle = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const std::string memberName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            const std::string dueDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            const int daysOverdue = computeDaysOverdue(dueDate);
            const double fineAmount = daysOverdue * FINE_RATE_PER_DAY;
            totalFines += fineAmount;

            std::cout << std::setw(10) << issueId
                      << std::setw(30) << bookTitle
                      << std::setw(20) << memberName
                      << std::setw(15) << daysOverdue
                      << std::setw(10) << fineAmount << "\n";
        }

        sqlite3_finalize(stmt);

        if (!hasFines) {
            std::cout << "\n[INFO] No outstanding fines. All books are returned or on time.\n";
            return;
        }

        std::cout << "====================================================================================================\n";
        std::cout << "Total Outstanding Fines: Rs. " << totalFines << "\n";
        std::cout << "====================================================================================================\n";
    }

    bool payFine(const std::string& fineId, const std::string& amountStr) {
        /*
         * VULNERABILITY: SQL Injection - fine_id and amount concatenated into query
         * VULNERABILITY: Improper Input Validation - accepts negative amounts
         * VULNERABILITY: Missing Authentication - no verification before payment
         */
        const std::string query =
            "SELECT fine_id, fine_amount, paid_amount, status FROM fines WHERE fine_id = " + fineId;

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Payment failed: " << sqlite3_errmsg(db_) << "\n";
            return false;
        }

        if (sqlite3_step(stmt) != SQLITE_ROW) {
            std::cout << "\n[ERROR] Fine record not found for ID: " << fineId << "\n";
            sqlite3_finalize(stmt);
            return false;
        }

        const int fineIdVal = sqlite3_column_int(stmt, 0);
        const double fineAmount = sqlite3_column_double(stmt, 1);
        const double paidAmount = sqlite3_column_double(stmt, 2);
        const std::string status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        sqlite3_finalize(stmt);

        const double paymentAmount = std::atof(amountStr.c_str());  // VULNERABLE: No validation
        const double newPaidAmount = paidAmount + paymentAmount;

        std::string newStatus = status;
        if (newPaidAmount >= fineAmount) {
            newStatus = "paid";
        } else if (newPaidAmount > 0) {
            newStatus = "partial";
        }

        const std::string updateQuery =
            "UPDATE fines SET paid_amount = " + std::to_string(newPaidAmount) +
            ", status = '" + newStatus + "' WHERE fine_id = " + fineId;

        char* errMsg = nullptr;
        if (sqlite3_exec(db_, updateQuery.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cout << "\n[ERROR] Payment failed: " << (errMsg ? errMsg : "unknown") << "\n";
            sqlite3_free(errMsg);
            return false;
        }

        std::cout << "\n[SUCCESS] Payment recorded!\n";
        std::cout << "Fine ID: " << fineIdVal << "\n";
        std::cout << "Payment Amount: Rs. " << paymentAmount << "\n";
        std::cout << "Total Paid: Rs. " << newPaidAmount << " / Rs. " << fineAmount << "\n";
        std::cout << "Status: " << newStatus << "\n";
        return true;
    }

    void searchByIsbn(const std::string& isbn) {
        /*
         * VULNERABILITY: SQL Injection - ISBN concatenated into query
         */
        const std::string query = "SELECT * FROM books WHERE isbn = '" + isbn + "'";

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] ISBN search failed: " << sqlite3_errmsg(db_) << "\n";
            return;
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const int availableCopies = sqlite3_column_int(stmt, 5);
            std::cout << "\n============================================================\n";
            std::cout << "BOOK DETAILS (ISBN Search)\n";
            std::cout << "============================================================\n";
            std::cout << "Book ID: " << sqlite3_column_int(stmt, 0) << "\n";
            std::cout << "Title: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) << "\n";
            std::cout << "Author: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) << "\n";
            std::cout << "ISBN: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) << "\n";
            std::cout << "Total Copies: " << sqlite3_column_int(stmt, 4) << "\n";
            std::cout << "Available Copies: " << availableCopies << "\n";
            std::cout << "Status: " << (availableCopies > 0 ? "Available" : "Not Available") << "\n";
            std::cout << "============================================================\n";
        } else {
            std::cout << "\n[INFO] No book found with ISBN: " << isbn << "\n";
        }

        sqlite3_finalize(stmt);
    }

    void advancedSearch(const std::string& title, const std::string& author, const std::string& availableOnly) {
        /*
         * VULNERABILITY: SQL Injection - all filter values concatenated into query
         */
        std::vector<std::string> conditions;

        if (!title.empty()) {
            conditions.push_back("title LIKE '%" + title + "%'");
        }
        if (!author.empty()) {
            conditions.push_back("author LIKE '%" + author + "%'");
        }

        const std::string availability = toLower(availableOnly);
        if (availability == "yes") {
            conditions.push_back("available_copies > 0");
        } else if (availability == "no") {
            conditions.push_back("available_copies = 0");
        }

        std::string whereClause = "1=1";
        if (!conditions.empty()) {
            whereClause = conditions[0];
            for (size_t i = 1; i < conditions.size(); ++i) {
                whereClause += " AND " + conditions[i];
            }
        }

        const std::string query = "SELECT * FROM books WHERE " + whereClause;

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "\n[ERROR] Advanced search failed: " << sqlite3_errmsg(db_) << "\n";
            return;
        }

        bool hasBooks = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!hasBooks) {
                std::cout << "\n================================================================================\n";
                std::cout << "Advanced Search Results\n";
                std::cout << "================================================================================\n";
                std::cout << std::left << std::setw(5) << "ID"
                          << std::setw(30) << "Title"
                          << std::setw(25) << "Author"
                          << std::setw(20) << "ISBN"
                          << std::setw(10) << "Available" << "\n";
                std::cout << "================================================================================\n";
                hasBooks = true;
            }

            std::cout << std::setw(5) << sqlite3_column_int(stmt, 0)
                      << std::setw(30) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
                      << std::setw(25) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))
                      << std::setw(20) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
                      << std::setw(10) << sqlite3_column_int(stmt, 5) << "\n";
        }

        if (hasBooks) {
            std::cout << "================================================================================\n";
        } else {
            std::cout << "\n[INFO] No books found matching the search criteria.\n";
        }

        sqlite3_finalize(stmt);
    }

    void close() {
        if (db_) {
            sqlite3_close(db_);
            db_ = nullptr;
        }
    }

private:
    std::string dbName_;
    sqlite3* db_;

    void initializeDatabase() {
        if (sqlite3_open(dbName_.c_str(), &db_) != SQLITE_OK) {
            std::cerr << "[FATAL] Cannot open database: " << sqlite3_errmsg(db_) << "\n";
            std::exit(1);
        }

        const char* createMembers =
            "CREATE TABLE IF NOT EXISTS members ("
            "member_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT NOT NULL,"
            "email TEXT NOT NULL,"
            "phone TEXT,"
            "address TEXT,"
            "registration_date TEXT)";

        const char* createBooks =
            "CREATE TABLE IF NOT EXISTS books ("
            "book_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "title TEXT NOT NULL,"
            "author TEXT NOT NULL,"
            "isbn TEXT,"
            "total_copies INTEGER,"
            "available_copies INTEGER)";

        const char* createIssuedBooks =
            "CREATE TABLE IF NOT EXISTS issued_books ("
            "issue_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "book_id INTEGER,"
            "member_id INTEGER,"
            "issue_date TEXT,"
            "due_date TEXT,"
            "return_date TEXT,"
            "status TEXT,"
            "FOREIGN KEY (book_id) REFERENCES books(book_id),"
            "FOREIGN KEY (member_id) REFERENCES members(member_id))";

        const char* createFines =
            "CREATE TABLE IF NOT EXISTS fines ("
            "fine_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "issue_id INTEGER,"
            "member_id INTEGER,"
            "fine_amount REAL,"
            "fine_date TEXT,"
            "paid_amount REAL DEFAULT 0,"
            "status TEXT DEFAULT 'unpaid',"
            "FOREIGN KEY (issue_id) REFERENCES issued_books(issue_id),"
            "FOREIGN KEY (member_id) REFERENCES members(member_id))";

        char* errMsg = nullptr;
        sqlite3_exec(db_, createMembers, nullptr, nullptr, &errMsg);
        sqlite3_free(errMsg);
        sqlite3_exec(db_, createBooks, nullptr, nullptr, &errMsg);
        sqlite3_free(errMsg);
        sqlite3_exec(db_, createIssuedBooks, nullptr, nullptr, &errMsg);
        sqlite3_free(errMsg);
        sqlite3_exec(db_, createFines, nullptr, nullptr, &errMsg);
        sqlite3_free(errMsg);

        addSampleData();
    }

    void addSampleData() {
        sqlite3_stmt* stmt = nullptr;

        if (sqlite3_prepare_v2(db_, "SELECT COUNT(*) FROM books", -1, &stmt, nullptr) == SQLITE_OK &&
            sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) == 0) {
            sqlite3_finalize(stmt);

            struct BookSample {
                const char* title;
                const char* author;
                const char* isbn;
                int total;
                int available;
            };

            const BookSample books[] = {
                {"The Great Gatsby", "F. Scott Fitzgerald", "978-0-7432-7356-5", 5, 5},
                {"To Kill a Mockingbird", "Harper Lee", "978-0-06-112008-4", 3, 3},
                {"1984", "George Orwell", "978-0-452-28423-4", 4, 4},
                {"Pride and Prejudice", "Jane Austen", "978-0-14-143951-8", 2, 2},
                {"The Catcher in the Rye", "J.D. Salinger", "978-0-316-76948-0", 3, 3}
            };

            for (const auto& book : books) {
                if (sqlite3_prepare_v2(
                        db_,
                        "INSERT INTO books (title, author, isbn, total_copies, available_copies) VALUES (?, ?, ?, ?, ?)",
                        -1, &stmt, nullptr) == SQLITE_OK) {
                    sqlite3_bind_text(stmt, 1, book.title, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 2, book.author, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 3, book.isbn, -1, SQLITE_STATIC);
                    sqlite3_bind_int(stmt, 4, book.total);
                    sqlite3_bind_int(stmt, 5, book.available);
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
            }
        } else {
            sqlite3_finalize(stmt);
        }

        if (sqlite3_prepare_v2(db_, "SELECT COUNT(*) FROM members", -1, &stmt, nullptr) == SQLITE_OK &&
            sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) == 0) {
            sqlite3_finalize(stmt);

            const std::string registrationDate = currentDate();
            if (sqlite3_prepare_v2(
                    db_,
                    "INSERT INTO members (name, email, phone, address, registration_date) VALUES (?, ?, ?, ?, ?)",
                    -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, "John Doe", -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 2, "john.doe@example.com", -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 3, "9876543210", -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 4, "123 Library Lane", -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 5, registrationDate.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        } else {
            sqlite3_finalize(stmt);
        }

        if (sqlite3_prepare_v2(db_, "SELECT COUNT(*) FROM issued_books", -1, &stmt, nullptr) == SQLITE_OK &&
            sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) == 0) {
            sqlite3_finalize(stmt);

            const std::string overdueIssueDate = dateOffsetDays(-30);
            const std::string overdueDueDate = dateOffsetDays(-10);

            if (sqlite3_prepare_v2(
                    db_,
                    "INSERT INTO issued_books (book_id, member_id, issue_date, due_date, status) VALUES (1, 1, ?, ?, 'issued')",
                    -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, overdueIssueDate.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 2, overdueDueDate.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }

            sqlite3_exec(db_, "UPDATE books SET available_copies = available_copies - 1 WHERE book_id = 1", nullptr, nullptr, nullptr);
        } else {
            sqlite3_finalize(stmt);
        }
    }

    static std::string currentDate() {
        return formatDate(std::chrono::system_clock::now());
    }

    static std::string dateOffsetDays(int days) {
        using namespace std::chrono;
        const auto offset = system_clock::now() + hours(24 * days);
        return formatDate(offset);
    }

    static std::string formatDate(const std::chrono::system_clock::time_point& timePoint) {
        const std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
        std::tm tm_buf{};
#ifdef _WIN32
        localtime_s(&tm_buf, &time);
#else
        localtime_r(&time, &tm_buf);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm_buf, "%Y-%m-%d");
        return oss.str();
    }

    static int computeDaysOverdue(const std::string& dueDateStr) {
        int year = 0;
        int month = 0;
        int day = 0;
        if (std::sscanf(dueDateStr.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
            return 0;
        }

        std::tm dueTm{};
        dueTm.tm_year = year - 1900;
        dueTm.tm_mon = month - 1;
        dueTm.tm_mday = day;
        dueTm.tm_hour = 0;
        dueTm.tm_min = 0;
        dueTm.tm_sec = 0;
        dueTm.tm_isdst = -1;
        const std::time_t dueTime = std::mktime(&dueTm);

        const std::time_t nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm todayTm{};
#ifdef _WIN32
        localtime_s(&todayTm, &nowTime);
#else
        localtime_r(&nowTime, &todayTm);
#endif
        todayTm.tm_hour = 0;
        todayTm.tm_min = 0;
        todayTm.tm_sec = 0;
        todayTm.tm_isdst = -1;
        const std::time_t todayTime = std::mktime(&todayTm);

        const double secondsDiff = std::difftime(todayTime, dueTime);
        const int days = static_cast<int>(secondsDiff / (60 * 60 * 24));
        return days > 0 ? days : 0;
    }

    static std::string toLower(std::string value) {
        std::transform(value.begin(), value.end(), value.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return value;
    }
};

static std::string trim(const std::string& value) {
    const auto start = value.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    const auto end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return trim(line);
}

int main() {
    std::cout << "============================================================\n";
    std::cout << "LIBRARY MANAGEMENT SYSTEM\n";
    std::cout << "Parts 1 & 2: Issue/Return, Members, Fines, Search\n";
    std::cout << "============================================================\n";

    LibraryManagement library;

    while (true) {
        std::cout << "\n============================================================\n";
        std::cout << "MENU\n";
        std::cout << "============================================================\n";
        std::cout << "1. Register New Member\n";
        std::cout << "2. View Member Information\n";
        std::cout << "3. List Available Books\n";
        std::cout << "4. Search Books\n";
        std::cout << "5. Issue Book\n";
        std::cout << "6. Return Book\n";
        std::cout << "7. View Issued Books\n";
        std::cout << "8. Calculate Fine for Issue\n";
        std::cout << "9. View All Outstanding Fines\n";
        std::cout << "10. Search Book by ISBN\n";
        std::cout << "11. Advanced Book Search\n";
        std::cout << "12. Pay Fine\n";
        std::cout << "13. Exit\n";
        std::cout << "============================================================\n";

        const std::string choice = readLine("\nEnter your choice (1-13): ");

        if (choice == "1") {
            std::cout << "\n--- Register New Member ---\n";
            const std::string name = readLine("Enter name: ");
            const std::string email = readLine("Enter email: ");
            const std::string phone = readLine("Enter phone: ");
            const std::string address = readLine("Enter address: ");
            library.registerMember(name, email, phone, address);
        } else if (choice == "2") {
            std::cout << "\n--- View Member Information ---\n";
            library.displayMemberInfo(readLine("Enter member ID: "));
        } else if (choice == "3") {
            std::cout << "\n--- Available Books ---\n";
            library.listAvailableBooks();
        } else if (choice == "4") {
            std::cout << "\n--- Search Books ---\n";
            library.searchBook(readLine("Enter book title or author name: "));
        } else if (choice == "5") {
            std::cout << "\n--- Issue Book ---\n";
            const std::string bookId = readLine("Enter book ID: ");
            const std::string memberId = readLine("Enter member ID: ");
            library.issueBook(bookId, memberId);
        } else if (choice == "6") {
            std::cout << "\n--- Return Book ---\n";
            library.returnBook(readLine("Enter issue ID: "));
        } else if (choice == "7") {
            std::cout << "\n--- View Issued Books ---\n";
            const std::string memberId = readLine("Enter member ID (or press Enter for all): ");
            library.viewIssuedBooks(memberId);
        } else if (choice == "8") {
            std::cout << "\n--- Calculate Fine for Issue ---\n";
            library.calculateFine(readLine("Enter issue ID: "));
        } else if (choice == "9") {
            std::cout << "\n--- View All Outstanding Fines ---\n";
            library.viewAllFines();
        } else if (choice == "10") {
            std::cout << "\n--- Search Book by ISBN ---\n";
            library.searchByIsbn(readLine("Enter ISBN: "));
        } else if (choice == "11") {
            std::cout << "\n--- Advanced Book Search ---\n";
            const std::string title = readLine("Enter title (or press Enter to skip): ");
            const std::string author = readLine("Enter author (or press Enter to skip): ");
            const std::string available = readLine("Available only? (yes/no/Enter for all): ");
            library.advancedSearch(title, author, available);
        } else if (choice == "12") {
            std::cout << "\n--- Pay Fine ---\n";
            const std::string fineId = readLine("Enter fine ID: ");
            const std::string amount = readLine("Enter payment amount: ");
            library.payFine(fineId, amount);
        } else if (choice == "13") {
            std::cout << "\n[INFO] Exiting Library Management System...\n";
            break;
        } else {
            std::cout << "\n[ERROR] Invalid choice! Please enter a number between 1 and 13.\n";
        }
    }

    return 0;
}
