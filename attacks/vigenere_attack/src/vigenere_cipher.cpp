#include "vigenere_cipher.h"
#include <cctype>

std::string VigenereCipher::vigenere_encrypt(const std::string& plaintext, const std::string& key) {
    std::string result = "";
    std::string cleanPlaintext = clean_ciphertext(plaintext);
    std::string upperKey = clean_ciphertext(key);
    
    if (upperKey.empty()) return cleanPlaintext;
    
    int keyIndex = 0;
    for (char c : cleanPlaintext) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            int shift = upperKey[keyIndex % upperKey.length()] - 'A';
            result += static_cast<char>((c - base + shift) % 26 + base);
            keyIndex++;
        } else {
            result += c;
        }
    }
    
    return result;
}

std::string VigenereCipher::vigenere_decrypt(const std::string& ciphertext, const std::string& key) {
    std::string result = "";
    std::string cleanCiphertext = clean_ciphertext(ciphertext);
    std::string upperKey = clean_ciphertext(key);
    
    if (upperKey.empty()) return cleanCiphertext;
    
    int keyIndex = 0;
    for (char c : cleanCiphertext) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            int shift = upperKey[keyIndex % upperKey.length()] - 'A';
            result += static_cast<char>((c - base - shift + 26) % 26 + base);
            keyIndex++;
        } else {
            result += c;
        }
    }
    
    return result;
}

std::string VigenereCipher::clean_ciphertext(const std::string& text) {
    std::string result = "";
    for (char c : text) {
        if (std::isalpha(c)) {
            result += std::toupper(c);
        }
    }
    return result;
}
