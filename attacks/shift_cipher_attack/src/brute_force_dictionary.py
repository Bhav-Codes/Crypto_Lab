"""
Brute Force Attack with Dictionary Scoring
Lab Assignment 4 - Group 4

This module implements a brute force attack on shift cipher using dictionary scoring.
It tries all possible keys (0-25) and scores each decryption based on how many
valid English words appear in the result.

The key with the highest score is likely the correct one.
"""

import os
from shift_cipher import ShiftCipher


class BruteForceDictionaryAttack:
    """
    Implements brute force attack with dictionary-based scoring.
    """
    
    def __init__(self, dictionary_path=None):
        """
        Initialize the attack with a dictionary file.
        
        Args:
            dictionary_path (str): Path to dictionary file. If None, uses default.
        """
        self.cipher = ShiftCipher()
        self.dictionary = set()
        
        if dictionary_path is None:
            # Default path relative to this file
            current_dir = os.path.dirname(os.path.abspath(__file__))
            dictionary_path = os.path.join(current_dir, '..', 'dictionary', 'english_words.txt')
        
        self.load_dictionary(dictionary_path)
    
    def load_dictionary(self, dictionary_path):
        """
        Load dictionary words from file.
        
        Args:
            dictionary_path (str): Path to dictionary file
        """
        try:
            with open(dictionary_path, 'r') as f:
                # Read all words and convert to uppercase
                self.dictionary = set(word.strip().upper() for word in f if word.strip())
            print(f"[INFO] Loaded {len(self.dictionary)} words from dictionary")
        except FileNotFoundError:
            print(f"[ERROR] Dictionary file not found: {dictionary_path}")
            print("[INFO] Using empty dictionary")
            self.dictionary = set()
    
    def calculate_word_score(self, text):
        """
        Calculate score based on number of valid English words.
        
        Args:
            text (str): The text to score
        
        Returns:
            int: Number of valid dictionary words found
        """
        # Remove non-alphabetic characters and split into words
        words = ''.join(c if c.isalpha() or c.isspace() else ' ' for c in text.upper()).split()
        
        # Count how many words are in the dictionary
        score = sum(1 for word in words if word in self.dictionary)
        
        return score
    
    def calculate_percentage_score(self, text):
        """
        Calculate percentage of words that are valid English words.
        
        Args:
            text (str): The text to score
        
        Returns:
            float: Percentage of valid words (0-100)
        """
        words = ''.join(c if c.isalpha() or c.isspace() else ' ' for c in text.upper()).split()
        
        if not words:
            return 0.0
        
        valid_words = sum(1 for word in words if word in self.dictionary)
        return (valid_words / len(words)) * 100
    
    def brute_force_attack(self, ciphertext, verbose=True):
        """
        Perform brute force attack trying all possible keys.
        
        Args:
            ciphertext (str): The encrypted text to attack
            verbose (bool): Whether to print detailed results
        
        Returns:
            dict: Results containing best key, plaintext, and all attempts
        """
        results = []
        
        if verbose:
            print("="*80)
            print("BRUTE FORCE ATTACK WITH DICTIONARY SCORING")
            print("="*80)
            print(f"Ciphertext: {ciphertext[:50]}{'...' if len(ciphertext) > 50 else ''}")
            print(f"Dictionary size: {len(self.dictionary)} words")
            print("\nTrying all keys (0-25)...\n")
            print(f"{'Key':<5} {'Score':<8} {'%Valid':<10} {'Plaintext':<50}")
            print("-" * 80)
        
        # Try all possible keys
        for key in range(26):
            # Decrypt with this key
            plaintext = self.cipher.decrypt_with_spaces(ciphertext, key)
            
            # Calculate scores
            word_score = self.calculate_word_score(plaintext)
            percentage_score = self.calculate_percentage_score(plaintext)
            
            results.append({
                'key': key,
                'plaintext': plaintext,
                'word_score': word_score,
                'percentage_score': percentage_score
            })
            
            if verbose:
                preview = plaintext[:50] + ('...' if len(plaintext) > 50 else '')
                print(f"{key:<5} {word_score:<8} {percentage_score:>6.1f}%   {preview}")
        
        # Find the best key (highest word score, then highest percentage)
        best_result = max(results, key=lambda x: (x['word_score'], x['percentage_score']))
        
        if verbose:
            print("\n" + "="*80)
            print("BEST RESULT")
            print("="*80)
            print(f"Predicted Key: {best_result['key']}")
            print(f"Word Score: {best_result['word_score']}")
            print(f"Percentage Valid: {best_result['percentage_score']:.1f}%")
            print(f"Decrypted Text: {best_result['plaintext']}")
            print("="*80)
        
        return {
            'best_key': best_result['key'],
            'best_plaintext': best_result['plaintext'],
            'best_score': best_result['word_score'],
            'best_percentage': best_result['percentage_score'],
            'all_results': results
        }
    
    def attack(self, ciphertext, verbose=True):
        """
        Convenience method for brute_force_attack.
        
        Args:
            ciphertext (str): The encrypted text to attack
            verbose (bool): Whether to print detailed results
        
        Returns:
            int: The predicted key
        """
        result = self.brute_force_attack(ciphertext, verbose)
        return result['best_key']


def demo():
    """Demonstrate brute force dictionary attack."""
    cipher = ShiftCipher()
    attacker = BruteForceDictionaryAttack()
    
    print("\n" + "="*80)
    print("DEMONSTRATION: BRUTE FORCE DICTIONARY ATTACK")
    print("="*80)
    
    # Test Case 1: Simple message
    plaintext1 = "ATTACK AT DAWN"
    actual_key1 = 7
    ciphertext1 = cipher.encrypt_with_spaces(plaintext1, actual_key1)
    
    print(f"\n{'='*80}")
    print("TEST CASE 1")
    print(f"{'='*80}")
    print(f"Original Plaintext: {plaintext1}")
    print(f"Actual Key: {actual_key1}")
    print(f"Ciphertext: {ciphertext1}\n")
    
    result1 = attacker.brute_force_attack(ciphertext1, verbose=False)
    
    print(f"\nPredicted Key: {result1['best_key']}")
    print(f"Decrypted Text: {result1['best_plaintext']}")
    print(f"Correct: {result1['best_key'] == actual_key1}")
    
    # Test Case 2: Longer message with more context
    plaintext2 = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG"
    actual_key2 = 13
    ciphertext2 = cipher.encrypt_with_spaces(plaintext2, actual_key2)
    
    print(f"\n{'='*80}")
    print("TEST CASE 2")
    print(f"{'='*80}")
    print(f"Original Plaintext: {plaintext2}")
    print(f"Actual Key: {actual_key2}")
    print(f"Ciphertext: {ciphertext2}\n")
    
    result2 = attacker.brute_force_attack(ciphertext2, verbose=False)
    
    print(f"\nPredicted Key: {result2['best_key']}")
    print(f"Decrypted Text: {result2['best_plaintext']}")
    print(f"Correct: {result2['best_key'] == actual_key2}")
    
    # Test Case 3: More realistic message
    plaintext3 = "CRYPTOGRAPHY IS THE PRACTICE AND STUDY OF TECHNIQUES FOR SECURE COMMUNICATION"
    actual_key3 = 5
    ciphertext3 = cipher.encrypt_with_spaces(plaintext3, actual_key3)
    
    print(f"\n{'='*80}")
    print("TEST CASE 3")
    print(f"{'='*80}")
    print(f"Original Plaintext: {plaintext3}")
    print(f"Actual Key: {actual_key3}")
    print(f"Ciphertext: {ciphertext3}\n")
    
    result3 = attacker.brute_force_attack(ciphertext3, verbose=True)


if __name__ == "__main__":
    demo()
