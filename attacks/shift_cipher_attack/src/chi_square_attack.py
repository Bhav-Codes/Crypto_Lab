"""
Chi-Square Statistical Attack on Shift Cipher
Lab Assignment 4 - Group 4

This module implements a chi-square statistical attack on shift cipher.
It uses letter frequency analysis to find the most likely key by comparing
the frequency distribution of letters in the decrypted text with the expected
frequency distribution of English letters.

The chi-square statistic measures how well the observed frequencies match
the expected frequencies. A lower chi-square value indicates a better match.
"""
a=3
from shift_cipher import ShiftCipher


class ChiSquareAttack:
    """
    Implements chi-square statistical attack on shift cipher.
    """
    
    # Expected frequency distribution of letters in English text (percentage)
    # Source: Based on analysis of large English text corpora
    ENGLISH_FREQ = {
        'A': 8.168, 'B': 1.492, 'C': 2.782, 'D': 4.253, 'E': 12.702,
        'F': 2.228, 'G': 2.015, 'H': 6.094, 'I': 6.966, 'J': 0.153,
        'K': 0.772, 'L': 4.025, 'M': 2.406, 'N': 6.749, 'O': 7.507,
        'P': 1.929, 'Q': 0.095, 'R': 5.977, 'S': 6.327, 'T': 9.056,
        'U': 2.758, 'V': 0.978, 'W': 2.360, 'X': 0.160, 'Y': 1.974,
        'Z': 0.074
    }
    
    def __init__(self):
        """Initialize the chi-square attack."""
        self.cipher = ShiftCipher()
        self.alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    
    def calculate_frequencies(self, text):
        """
        Calculate the frequency of each letter in the text.
        
        Args:
            text (str): The text to analyze
        
        Returns:
            dict: Dictionary mapping each letter to its frequency (percentage)
        """
        # Remove non-alphabetic characters and convert to uppercase
        text = ''.join(c for c in text.upper() if c.isalpha())
        
        # Count total letters
        total_letters = len(text)
        
        if total_letters == 0:
            return {letter: 0.0 for letter in self.alphabet}
        
        # Count frequency of each letter
        letter_counts = {}
        for letter in self.alphabet:
            letter_counts[letter] = text.count(letter)
        
        # Convert counts to percentages
        frequencies = {}
        for letter in self.alphabet:
            frequencies[letter] = (letter_counts[letter] / total_letters) * 100
        
        return frequencies
    
    def calculate_chi_square(self, observed_freq):
        """
        Calculate the chi-square statistic comparing observed vs expected frequencies.
        
        The formula is: χ² = Σ ((Observed - Expected)² / Expected)
        
        Args:
            observed_freq (dict): Observed letter frequencies
        
        Returns:
            float: Chi-square statistic (lower is better)
        """
        chi_square = 0.0
        
        for letter in self.alphabet:
            observed = observed_freq[letter]
            expected = self.ENGLISH_FREQ[letter]
            
            # Avoid division by zero
            if expected > 0:
                chi_square += ((observed - expected) ** 2) / expected
        
        return chi_square
    
    def chi_square_attack(self, ciphertext, verbose=True):
        """
        Perform chi-square statistical attack on the ciphertext.
        
        Args:
            ciphertext (str): The encrypted text to attack
            verbose (bool): Whether to print detailed results
        
        Returns:
            dict: Results containing best key, plaintext, and all attempts
        """
        results = []
        
        if verbose:
            print("="*80)
            print("CHI-SQUARE STATISTICAL ATTACK")
            print("="*80)
            print(f"Ciphertext: {ciphertext[:50]}{'...' if len(ciphertext) > 50 else ''}")
            print(f"\nAnalyzing letter frequencies for all possible keys...")
            print("\nExpected English letter frequencies:")
            print("E(12.70%), T(9.06%), A(8.17%), O(7.51%), I(6.97%), N(6.75%)...")
            print("\n" + f"{'Key':<5} {'Chi-Square':<15} {'Plaintext':<50}")
            print("-" * 80)
        
        # Try all possible keys
        for key in range(26):
            # Decrypt with this key
            plaintext = self.cipher.decrypt_with_spaces(ciphertext, key)
            
            # Calculate letter frequencies
            observed_freq = self.calculate_frequencies(plaintext)
            
            # Calculate chi-square statistic
            chi_square = self.calculate_chi_square(observed_freq)
            
            results.append({
                'key': key,
                'plaintext': plaintext,
                'chi_square': chi_square,
                'frequencies': observed_freq
            })
            
            if verbose:
                preview = plaintext[:50] + ('...' if len(plaintext) > 50 else '')
                print(f"{key:<5} {chi_square:<15.4f} {preview}")
        
        # Find the best key (lowest chi-square value)
        best_result = min(results, key=lambda x: x['chi_square'])
        
        if verbose:
            print("\n" + "="*80)
            print("BEST RESULT")
            print("="*80)
            print(f"Predicted Key: {best_result['key']}")
            print(f"Chi-Square Value: {best_result['chi_square']:.4f}")
            print(f"Decrypted Text: {best_result['plaintext']}")
            
            # Show top 5 letter frequencies in decrypted text
            sorted_freq = sorted(best_result['frequencies'].items(), 
                                key=lambda x: x[1], reverse=True)
            print(f"\nTop 5 letters in decrypted text:")
            for letter, freq in sorted_freq[:5]:
                expected = self.ENGLISH_FREQ[letter]
                print(f"  {letter}: {freq:.2f}% (expected: {expected:.2f}%)")
            
            print("="*80)
        
        return {
            'best_key': best_result['key'],
            'best_plaintext': best_result['plaintext'],
            'best_chi_square': best_result['chi_square'],
            'best_frequencies': best_result['frequencies'],
            'all_results': results
        }
    
    def attack(self, ciphertext, verbose=True):
        """
        Convenience method for chi_square_attack.
        
        Args:
            ciphertext (str): The encrypted text to attack
            verbose (bool): Whether to print detailed results
        
        Returns:
            int: The predicted key
        """
        result = self.chi_square_attack(ciphertext, verbose)
        return result['best_key']
    
    def compare_frequencies(self, text):
        """
        Compare letter frequencies in text with expected English frequencies.
        
        Args:
            text (str): The text to analyze
        """
        observed = self.calculate_frequencies(text)
        
        print("\n" + "="*80)
        print("LETTER FREQUENCY COMPARISON")
        print("="*80)
        print(f"{'Letter':<10} {'Observed':<15} {'Expected':<15} {'Difference':<15}")
        print("-" * 80)
        
        for letter in self.alphabet:
            obs = observed[letter]
            exp = self.ENGLISH_FREQ[letter]
            diff = obs - exp
            print(f"{letter:<10} {obs:>6.2f}%{'':<9} {exp:>6.2f}%{'':<9} {diff:>+6.2f}%{'':<9}")
        
        chi_square = self.calculate_chi_square(observed)
        print("-" * 80)
        print(f"Chi-Square Statistic: {chi_square:.4f}")
        print(f"(Lower values indicate better match with English)")
        print("="*80)


def demo():
    """Demonstrate chi-square statistical attack."""
    cipher = ShiftCipher()
    attacker = ChiSquareAttack()
    
    print("\n" + "="*80)
    print("DEMONSTRATION: CHI-SQUARE STATISTICAL ATTACK")
    print("="*80)
    
    # Test Case 1: Simple message
    plaintext1 = "ATTACK AT DAWN"
    actual_key1 = 7
    ciphertext1 = cipher.encrypt_with_spaces(plaintext1, actual_key1)
    
    print(f"\n{'='*80}")
    print("TEST CASE 1 - Short Message")
    print(f"{'='*80}")
    print(f"Original Plaintext: {plaintext1}")
    print(f"Actual Key: {actual_key1}")
    print(f"Ciphertext: {ciphertext1}\n")
    
    result1 = attacker.chi_square_attack(ciphertext1, verbose=False)
    
    print(f"\nPredicted Key: {result1['best_key']}")
    print(f"Chi-Square Value: {result1['best_chi_square']:.4f}")
    print(f"Decrypted Text: {result1['best_plaintext']}")
    print(f"Correct: {result1['best_key'] == actual_key1}")
    
    # Test Case 2: Longer message for better statistical analysis
    plaintext2 = "CRYPTOGRAPHY IS THE PRACTICE AND STUDY OF TECHNIQUES FOR SECURE COMMUNICATION IN THE PRESENCE OF THIRD PARTIES CALLED ADVERSARIES"
    actual_key2 = 13
    ciphertext2 = cipher.encrypt_with_spaces(plaintext2, actual_key2)
    
    print(f"\n{'='*80}")
    print("TEST CASE 2 - Longer Message (Better Statistics)")
    print(f"{'='*80}")
    print(f"Original Plaintext: {plaintext2[:60]}...")
    print(f"Actual Key: {actual_key2}")
    print(f"Ciphertext: {ciphertext2[:60]}...\n")
    
    result2 = attacker.chi_square_attack(ciphertext2, verbose=True)
    
    print(f"\nCorrect: {result2['best_key'] == actual_key2}")
    
    # Show frequency comparison for the correctly decrypted text
    print(f"\n{'='*80}")
    print("FREQUENCY ANALYSIS OF DECRYPTED TEXT")
    print(f"{'='*80}")
    attacker.compare_frequencies(result2['best_plaintext'])


if __name__ == "__main__":
    demo()
