#!/usr/bin/env python3
"""
CryptoLabX - A Cryptanalysis Toolkit
Main entry point for the application
"""

import os
import sys
from datetime import datetime
from utils.logger import Logger
from utils.file_analyzer import FileAnalyzer


class CryptoLabX:
    """Main application class for CryptoLabX toolkit"""
    
    def __init__(self):
        self.logger = Logger()
        self.file_analyzer = FileAnalyzer()
        self.running = True
    
    def display_menu(self):
        """Display the main menu"""
        print("\n" + "="*50)
        print("        CryptoLabX - Cryptanalysis Toolkit")
        print("="*50)
        print("1. Encrypt")
        print("2. Decrypt")
        print("3. Attack")
        print("4. Analyze")
        print("5. Exit")
        print("="*50)
    
    def handle_encrypt(self):
        """Handle encryption option"""
        print("\n[Encrypt] Coming Soon...")
        self.logger.log("Encrypt")
    
    def handle_decrypt(self):
        """Handle decryption option"""
        print("\n[Decrypt] Coming Soon...")
        self.logger.log("Decrypt")
    
    def handle_attack(self):
        """Handle attack option"""
        print("\n[Attack] Coming Soon...")
        self.logger.log("Attack")
    
    def handle_analyze(self):
        """Handle file analysis option"""
        print("\n[Analyze] File Analysis")
        self.logger.log("Analyze")
        
        # List available files in datasets folder
        datasets_path = os.path.join(os.path.dirname(__file__), "datasets")
        
        if not os.path.exists(datasets_path):
            print("Error: datasets folder not found!")
            return
        
        files = [f for f in os.listdir(datasets_path) if f.endswith('.txt')]
        
        if not files:
            print("No text files found in datasets folder!")
            return
        
        print("\nAvailable files:")
        for i, file in enumerate(files, 1):
            print(f"{i}. {file}")
        
        try:
            choice = input("\nSelect a file number (or press Enter to cancel): ").strip()
            if not choice:
                return
            
            file_index = int(choice) - 1
            if 0 <= file_index < len(files):
                file_path = os.path.join(datasets_path, files[file_index])
                self.file_analyzer.analyze_file(file_path)
            else:
                print("Invalid selection!")
        except ValueError:
            print("Invalid input!")
        except Exception as e:
            print(f"Error: {e}")
    
    def handle_exit(self):
        """Handle exit option"""
        print("\nThank you for using CryptoLabX!")
        self.logger.log("Exit")
        self.running = False
    
    def run(self):
        """Main application loop"""
        print("Welcome to CryptoLabX!")
        
        while self.running:
            self.display_menu()
            choice = input("\nEnter your choice (1-5): ").strip()
            
            if choice == '1':
                self.handle_encrypt()
            elif choice == '2':
                self.handle_decrypt()
            elif choice == '3':
                self.handle_attack()
            elif choice == '4':
                self.handle_analyze()
            elif choice == '5':
                self.handle_exit()
            else:
                print("\nInvalid choice! Please select 1-5.")


def main():
    """Entry point of the application"""
    try:
        app = CryptoLabX()
        app.run()
    except KeyboardInterrupt:
        print("\n\nProgram interrupted by user. Exiting...")
        sys.exit(0)
    except Exception as e:
        print(f"\nAn error occurred: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
