"""
Shift Cipher Implementation (Caesar Cipher)
Lab Assignment 4 - Group 4

This module implements the basic shift cipher encryption and decryption.
A shift cipher shifts each letter in the plaintext by a fixed number of positions
in the alphabet.

Example:
    Plaintext: "HELLO"
    Key: 3
    Ciphertext: "KHOOR"
    !!!!!
"""


class ShiftCipher:
    """
    Implements shift cipher (Caesar cipher) encryption and decryption.
    """
    
    def __init__(self):
        """Initialize the shift cipher."""
        self.alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        self.alphabet_size = 26
    
    def encrypt(self, plaintext, key):
        """
        Encrypt plaintext using shift cipher.
        
        Args:
            plaintext (str): The text to encrypt
            key (int): The shift amount (0-25)
        
        Returns:
            str: The encrypted ciphertext
        
        Example:
            >>> cipher = ShiftCipher()
            >>> cipher.encrypt("HELLO", 3)
            'KHOOR'
        """
        if not isinstance(key, int) or key < 0 or key >= self.alphabet_size:
            raise ValueError(f"Key must be an integer between 0 and {self.alphabet_size - 1}")
        
        ciphertext = []
        plaintext = plaintext.upper()
        
        for char in plaintext:
            if char in self.alphabet:
                # Find position of character in alphabet
                old_index = self.alphabet.index(char)
                # Shift by key amount (with wraparound)
                new_index = (old_index + key) % self.alphabet_size
                # Append encrypted character
                ciphertext.append(self.alphabet[new_index])
            else:
                # Non-alphabetic characters remain unchanged
                ciphertext.append(char)
        
        return ''.join(ciphertext)
    
    def decrypt(self, ciphertext, key):
        """
        Decrypt ciphertext using shift cipher.
        
        Args:
            ciphertext (str): The text to decrypt
            key (int): The shift amount used for encryption (0-25)
        
        Returns:
            str: The decrypted plaintext
        
        Example:
            >>> cipher = ShiftCipher()
            >>> cipher.decrypt("KHOOR", 3)
            'HELLO'
        """
        if not isinstance(key, int) or key < 0 or key >= self.alphabet_size:
            raise ValueError(f"Key must be an integer between 0 and {self.alphabet_size - 1}")
        
        plaintext = []
        ciphertext = ciphertext.upper()
        
        for char in ciphertext:
            if char in self.alphabet:
                # Find position of character in alphabet
                old_index = self.alphabet.index(char)
                # Shift back by key amount (with wraparound)
                new_index = (old_index - key) % self.alphabet_size
                # Append decrypted character
                plaintext.append(self.alphabet[new_index])
            else:
                # Non-alphabetic characters remain unchanged
                plaintext.append(char)
        
        return ''.join(plaintext)
    
    def encrypt_with_spaces(self, plaintext, key):
        """
        Encrypt plaintext while preserving spaces and punctuation.
        
        Args:
            plaintext (str): The text to encrypt
            key (int): The shift amount (0-25)
        
        Returns:
            str: The encrypted ciphertext with spaces preserved
        """
        result = []
        plaintext_upper = plaintext.upper()
        
        for char in plaintext_upper:
            if char in self.alphabet:
                old_index = self.alphabet.index(char)
                new_index = (old_index + key) % self.alphabet_size
                result.append(self.alphabet[new_index])
            else:
                result.append(char)
        
        return ''.join(result)
    
    def decrypt_with_spaces(self, ciphertext, key):
        """
        Decrypt ciphertext while preserving spaces and punctuation.
        
        Args:
            ciphertext (str): The text to decrypt
            key (int): The shift amount (0-25)
        
        Returns:
            str: The decrypted plaintext with spaces preserved
        """
        result = []
        ciphertext_upper = ciphertext.upper()
        
        for char in ciphertext_upper:
            if char in self.alphabet:
                old_index = self.alphabet.index(char)
                new_index = (old_index - key) % self.alphabet_size
                result.append(self.alphabet[new_index])
            else:
                result.append(char)
        
        return ''.join(result)


def demo():
    """Demonstrate shift cipher encryption and decryption."""
    cipher = ShiftCipher()
    
    print("="*60)
    print("SHIFT CIPHER DEMONSTRATION")
    print("="*60)
    
    # Example 1
    plaintext1 = "HELLO WORLD"
    key1 = 3
    ciphertext1 = cipher.encrypt_with_spaces(plaintext1, key1)
    decrypted1 = cipher.decrypt_with_spaces(ciphertext1, key1)
    
    print(f"\nExample 1:")
    print(f"Plaintext:  {plaintext1}")
    print(f"Key:        {key1}")
    print(f"Ciphertext: {ciphertext1}")
    print(f"Decrypted:  {decrypted1}")
    print(f"Match:      {plaintext1 == decrypted1}")
    
    # Example 2
    plaintext2 = "CRYPTOGRAPHY IS FUN"
    key2 = 13
    ciphertext2 = cipher.encrypt_with_spaces(plaintext2, key2)
    decrypted2 = cipher.decrypt_with_spaces(ciphertext2, key2)
    
    print(f"\nExample 2 (ROT13):")
    print(f"Plaintext:  {plaintext2}")
    print(f"Key:        {key2}")
    print(f"Ciphertext: {ciphertext2}")
    print(f"Decrypted:  {decrypted2}")
    print(f"Match:      {plaintext2 == decrypted2}")
    
    # Example 3 - All possible shifts
    plaintext3 = "ATTACK AT DAWN"
    print(f"\nExample 3 - All possible encryptions:")
    print(f"Plaintext: {plaintext3}")
    print(f"\n{'Key':<5} {'Ciphertext':<30}")
    print("-" * 35)
    for key in range(26):
        ciphertext = cipher.encrypt_with_spaces(plaintext3, key)
        print(f"{key:<5} {ciphertext:<30}")
    
    print("\n" + "="*60)


if __name__ == "__main__":
    demo()
