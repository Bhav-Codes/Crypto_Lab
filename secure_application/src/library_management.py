"""
Library Management System - Part 1
Handles: Book Issue/Return and Member Registration
Group 4 - Assignment 3

WARNING: This application contains intentional security vulnerabilities for educational purposes.
Vulnerabilities included:
1. SQL Injection in member registration and book search
2. Cross-Site Scripting (XSS) in display functions
3. Missing Authentication checks in critical operations

DO NOT use this code in production!
"""

import sqlite3
import datetime
import os

class LibraryManagement:
    def __init__(self, db_name="library.db"):
        self.db_name = db_name
        self.connection = None
        self.initialize_database()
    
    def initialize_database(self):
        """Initialize the database with required tables"""
        self.connection = sqlite3.connect(self.db_name)
        cursor = self.connection.cursor()
        
        # Create members table
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS members (
                member_id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                email TEXT NOT NULL,
                phone TEXT,
                address TEXT,
                registration_date TEXT
            )
        """)
        
        # Create books table
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS books (
                book_id INTEGER PRIMARY KEY AUTOINCREMENT,
                title TEXT NOT NULL,
                author TEXT NOT NULL,
                isbn TEXT,
                total_copies INTEGER,
                available_copies INTEGER
            )
        """)
        
        # Create issued_books table
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS issued_books (
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
        """)
        
        self.connection.commit()
        self.add_sample_data()
    
    def add_sample_data(self):
        """Add some sample books for testing"""
        cursor = self.connection.cursor()
        
        # Check if books already exist
        cursor.execute("SELECT COUNT(*) FROM books")
        if cursor.fetchone()[0] == 0:
            sample_books = [
                ("The Great Gatsby", "F. Scott Fitzgerald", "978-0-7432-7356-5", 5, 5),
                ("To Kill a Mockingbird", "Harper Lee", "978-0-06-112008-4", 3, 3),
                ("1984", "George Orwell", "978-0-452-28423-4", 4, 4),
                ("Pride and Prejudice", "Jane Austen", "978-0-14-143951-8", 2, 2),
                ("The Catcher in the Rye", "J.D. Salinger", "978-0-316-76948-0", 3, 3)
            ]
            
            cursor.executemany("""
                INSERT INTO books (title, author, isbn, total_copies, available_copies)
                VALUES (?, ?, ?, ?, ?)
            """, sample_books)
            
            self.connection.commit()
    
    def register_member(self, name, email, phone, address):
        """
        Register a new member
        VULNERABILITY: SQL Injection - User input is directly concatenated into SQL query
        """
        cursor = self.connection.cursor()
        registration_date = datetime.datetime.now().strftime("%Y-%m-%d")
        
        # VULNERABLE: SQL Injection vulnerability
        query = f"INSERT INTO members (name, email, phone, address, registration_date) VALUES ('{name}', '{email}', '{phone}', '{address}', '{registration_date}')"
        
        try:
            cursor.execute(query)
            self.connection.commit()
            member_id = cursor.lastrowid
            print(f"\n[SUCCESS] Member registered successfully!")
            print(f"Member ID: {member_id}")
            print(f"Name: {name}")
            print(f"Email: {email}")
            return member_id
        except Exception as e:
            print(f"\n[ERROR] Registration failed: {e}")
            return None
    
    def display_member_info(self, member_id):
        """
        Display member information
        VULNERABILITY: XSS - User input displayed without sanitization
        """
        cursor = self.connection.cursor()
        
        # VULNERABLE: SQL Injection in search
        query = f"SELECT * FROM members WHERE member_id = {member_id}"
        cursor.execute(query)
        member = cursor.fetchone()
        
        if member:
            # VULNERABLE: XSS - displaying unsanitized user input
            print(f"\n{'='*50}")
            print(f"Member ID: {member[0]}")
            print(f"Name: {member[1]}")  # XSS vulnerability
            print(f"Email: {member[2]}")
            print(f"Phone: {member[3]}")
            print(f"Address: {member[4]}")
            print(f"Registration Date: {member[5]}")
            print(f"{'='*50}")
            return member
        else:
            print(f"\n[ERROR] Member not found!")
            return None
    
    def list_available_books(self):
        """List all available books"""
        cursor = self.connection.cursor()
        cursor.execute("SELECT * FROM books WHERE available_copies > 0")
        books = cursor.fetchall()
        
        if books:
            print(f"\n{'='*80}")
            print(f"{'ID':<5} {'Title':<30} {'Author':<25} {'Available':<10}")
            print(f"{'='*80}")
            for book in books:
                print(f"{book[0]:<5} {book[1]:<30} {book[2]:<25} {book[5]:<10}")
            print(f"{'='*80}")
        else:
            print("\n[INFO] No books available at the moment.")
        
        return books
    
    def search_book(self, search_term):
        """
        Search for books by title or author
        VULNERABILITY: SQL Injection in search query
        """
        cursor = self.connection.cursor()
        
        # VULNERABLE: SQL Injection
        query = f"SELECT * FROM books WHERE title LIKE '%{search_term}%' OR author LIKE '%{search_term}%'"
        
        try:
            cursor.execute(query)
            books = cursor.fetchall()
            
            if books:
                print(f"\n{'='*80}")
                print(f"Search Results for: {search_term}")
                print(f"{'='*80}")
                print(f"{'ID':<5} {'Title':<30} {'Author':<25} {'Available':<10}")
                print(f"{'='*80}")
                for book in books:
                    print(f"{book[0]:<5} {book[1]:<30} {book[2]:<25} {book[5]:<10}")
                print(f"{'='*80}")
                return books
            else:
                print(f"\n[INFO] No books found matching '{search_term}'")
                return []
        except Exception as e:
            print(f"\n[ERROR] Search failed: {e}")
            return []
    
    def issue_book(self, book_id, member_id):
        """
        Issue a book to a member
        VULNERABILITY: Missing Authentication - No check if user is authorized
        """
        cursor = self.connection.cursor()
        
        # VULNERABLE: No authentication check
        # In a secure system, we should verify the user's identity and authorization
        
        # Check if book exists and is available
        cursor.execute("SELECT * FROM books WHERE book_id = ? AND available_copies > 0", (book_id,))
        book = cursor.fetchone()
        
        if not book:
            print(f"\n[ERROR] Book not available or doesn't exist!")
            return False
        
        # Check if member exists
        cursor.execute("SELECT * FROM members WHERE member_id = ?", (member_id,))
        member = cursor.fetchone()
        
        if not member:
            print(f"\n[ERROR] Member not found!")
            return False
        
        # Calculate dates
        issue_date = datetime.datetime.now().strftime("%Y-%m-%d")
        due_date = (datetime.datetime.now() + datetime.timedelta(days=14)).strftime("%Y-%m-%d")
        
        try:
            # Issue the book
            cursor.execute("""
                INSERT INTO issued_books (book_id, member_id, issue_date, due_date, status)
                VALUES (?, ?, ?, ?, 'issued')
            """, (book_id, member_id, issue_date, due_date))
            
            # Update available copies
            cursor.execute("""
                UPDATE books SET available_copies = available_copies - 1
                WHERE book_id = ?
            """, (book_id,))
            
            self.connection.commit()
            
            print(f"\n[SUCCESS] Book issued successfully!")
            print(f"Book: {book[1]} by {book[2]}")
            print(f"Member: {member[1]}")
            print(f"Issue Date: {issue_date}")
            print(f"Due Date: {due_date}")
            
            return True
        except Exception as e:
            print(f"\n[ERROR] Failed to issue book: {e}")
            self.connection.rollback()
            return False
    
    def return_book(self, issue_id):
        """
        Return a book
        VULNERABILITY: Missing Authentication - No verification of who is returning
        """
        cursor = self.connection.cursor()
        
        # VULNERABLE: No authentication check
        # Anyone can return any book without verification
        
        # Check if issue exists and book is not already returned
        cursor.execute("""
            SELECT * FROM issued_books WHERE issue_id = ? AND status = 'issued'
        """, (issue_id,))
        issue = cursor.fetchone()
        
        if not issue:
            print(f"\n[ERROR] Issue record not found or book already returned!")
            return False
        
        book_id = issue[1]
        return_date = datetime.datetime.now().strftime("%Y-%m-%d")
        
        try:
            # Update issue record
            cursor.execute("""
                UPDATE issued_books SET return_date = ?, status = 'returned'
                WHERE issue_id = ?
            """, (return_date, issue_id))
            
            # Update available copies
            cursor.execute("""
                UPDATE books SET available_copies = available_copies + 1
                WHERE book_id = ?
            """, (book_id,))
            
            self.connection.commit()
            
            print(f"\n[SUCCESS] Book returned successfully!")
            print(f"Issue ID: {issue_id}")
            print(f"Return Date: {return_date}")
            
            return True
        except Exception as e:
            print(f"\n[ERROR] Failed to return book: {e}")
            self.connection.rollback()
            return False
    
    def view_issued_books(self, member_id=None):
        """View all issued books or issued books for a specific member"""
        cursor = self.connection.cursor()
        
        if member_id:
            cursor.execute("""
                SELECT i.issue_id, b.title, b.author, m.name, i.issue_date, i.due_date, i.status
                FROM issued_books i
                JOIN books b ON i.book_id = b.book_id
                JOIN members m ON i.member_id = m.member_id
                WHERE i.member_id = ? AND i.status = 'issued'
            """, (member_id,))
        else:
            cursor.execute("""
                SELECT i.issue_id, b.title, b.author, m.name, i.issue_date, i.due_date, i.status
                FROM issued_books i
                JOIN books b ON i.book_id = b.book_id
                JOIN members m ON i.member_id = m.member_id
                WHERE i.status = 'issued'
            """)
        
        issues = cursor.fetchall()
        
        if issues:
            print(f"\n{'='*100}")
            print(f"{'Issue ID':<10} {'Book Title':<30} {'Author':<20} {'Member':<20} {'Due Date':<12}")
            print(f"{'='*100}")
            for issue in issues:
                print(f"{issue[0]:<10} {issue[1]:<30} {issue[2]:<20} {issue[3]:<20} {issue[5]:<12}")
            print(f"{'='*100}")
        else:
            print("\n[INFO] No issued books found.")
        
        return issues
    
    def close(self):
        """Close database connection"""
        if self.connection:
            self.connection.close()


def main():
    """Main function with menu-driven interface"""
    print("="*60)
    print("LIBRARY MANAGEMENT SYSTEM - PART 1")
    print("Book Issue/Return & Member Registration")
    print("="*60)
    
    library = LibraryManagement()
    
    while True:
        print("\n" + "="*60)
        print("MENU")
        print("="*60)
        print("1. Register New Member")
        print("2. View Member Information")
        print("3. List Available Books")
        print("4. Search Books")
        print("5. Issue Book")
        print("6. Return Book")
        print("7. View Issued Books")
        print("8. Exit")
        print("="*60)
        
        choice = input("\nEnter your choice (1-8): ").strip()
        
        if choice == "1":
            print("\n--- Register New Member ---")
            name = input("Enter name: ")
            email = input("Enter email: ")
            phone = input("Enter phone: ")
            address = input("Enter address: ")
            library.register_member(name, email, phone, address)
        
        elif choice == "2":
            print("\n--- View Member Information ---")
            member_id = input("Enter member ID: ")
            library.display_member_info(member_id)
        
        elif choice == "3":
            print("\n--- Available Books ---")
            library.list_available_books()
        
        elif choice == "4":
            print("\n--- Search Books ---")
            search_term = input("Enter book title or author name: ")
            library.search_book(search_term)
        
        elif choice == "5":
            print("\n--- Issue Book ---")
            book_id = input("Enter book ID: ")
            member_id = input("Enter member ID: ")
            library.issue_book(book_id, member_id)
        
        elif choice == "6":
            print("\n--- Return Book ---")
            issue_id = input("Enter issue ID: ")
            library.return_book(issue_id)
        
        elif choice == "7":
            print("\n--- View Issued Books ---")
            member_id = input("Enter member ID (or press Enter for all): ").strip()
            if member_id:
                library.view_issued_books(int(member_id))
            else:
                library.view_issued_books()
        
        elif choice == "8":
            print("\n[INFO] Exiting Library Management System...")
            library.close()
            break
        
        else:
            print("\n[ERROR] Invalid choice! Please enter a number between 1 and 8.")


if __name__ == "__main__":
    main()
