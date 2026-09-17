#ifndef VIGENERE_CIPHER_H
#define VIGENERE_CIPHER_H

#include <string>
#include <vector>

class VigenereCipher {
public:
    // Encryption and Decryption
    static std::string vigenere_encrypt(const std::string& plaintext, const std::string& key);
    static std::string vigenere_decrypt(const std::string& ciphertext, const std::string& key);
    
    // Preprocessing
    static std::string clean_ciphertext(const std::string& text);
};

#endif
