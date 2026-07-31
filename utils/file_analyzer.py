"""
File analysis utility for text files
"""

import os
from collections import Counter


class FileAnalyzer:
    """Analyzes text files and provides statistics"""
    
    def __init__(self):
        pass
    
    def analyze_file(self, file_path):
        """
        Analyze a text file and display statistics
        
        Args:
            file_path (str): Path to the text file
        """
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Calculate statistics
            num_characters = len(content)
            num_lines = content.count('\n') + (1 if content and not content.endswith('\n') else 0)
            num_words = len(content.split())
            
            # Get unique characters (excluding whitespace)
            unique_chars = set(content)
            num_unique_chars = len(unique_chars)
            
            # Calculate letter frequency (only alphabetic characters)
            letters = [char.upper() for char in content if char.isalpha()]
            letter_freq = Counter(letters)
            
            # Display results
            print("\n" + "="*60)
            print(f"File Analysis: {os.path.basename(file_path)}")
            print("="*60)
            print(f"Total Characters: {num_characters}")
            print(f"Total Words: {num_words}")
            print(f"Total Lines: {num_lines}")
            print(f"Unique Characters: {num_unique_chars}")
            
            print("\nLetter Frequency Distribution:")
            print("-"*60)
            
            # Sort by frequency (descending)
            sorted_freq = sorted(letter_freq.items(), key=lambda x: x[1], reverse=True)
            
            # Display in columns
            for i in range(0, len(sorted_freq), 3):
                row = sorted_freq[i:i+3]
                line = "  ".join([f"{letter}: {count:4d}" for letter, count in row])
                print(line)
            
            # Calculate percentages for top 5
            total_letters = sum(letter_freq.values())
            if total_letters > 0:
                print("\nTop 5 Most Frequent Letters:")
                print("-"*60)
                for letter, count in sorted_freq[:5]:
                    percentage = (count / total_letters) * 100
                    print(f"{letter}: {count:4d} ({percentage:5.2f}%)")
            
            print("="*60)
            
        except FileNotFoundError:
            print(f"Error: File '{file_path}' not found!")
        except Exception as e:
            print(f"Error analyzing file: {e}")
