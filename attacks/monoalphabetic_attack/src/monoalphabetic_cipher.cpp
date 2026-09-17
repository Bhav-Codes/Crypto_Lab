#include "monoalphabetic_cipher.h"
#include <algorithm>
#include <random>
#include <chrono>

MonoalphabeticCipher::MonoalphabeticCipher() {
    // Initialize with identity mapping
    for (int i = 0; i < 26; i++) {
        key[i] = 'A' + i;
    }
}

void MonoalphabeticCipher::generateRandomKey() {
    // Create alphabet array
    for (int i = 0; i < 26; i++) {
        key[i] = 'A' + i;
    }
    
    // Shuffle using random device
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(key, key + 26, std::default_random_engine(seed));
}

void MonoalphabeticCipher::setKey(const std::string& keyStr) {
    if (keyStr.length() != 26) {
        throw std::invalid_argument("Key must be exactly 26 characters");
    }
    
    // Check if all letters are unique
    bool used[26] = {false};
    for (char c : keyStr) {
        char upper = std::toupper(c);
        if (upper < 'A' || upper > 'Z') {
            throw std::invalid_argument("Key must contain only letters");
        }
        int index = upper - 'A';
        if (used[index]) {
            throw std::invalid_argument("Key must contain unique letters");
        }
        used[index] = true;
    }
    
    for (int i = 0; i < 26; i++) {
        key[i] = std::toupper(keyStr[i]);
    }
}

std::string MonoalphabeticCipher::getKey() const {
    return std::string(key, 26);
}

std::string MonoalphabeticCipher::encrypt(const std::string& plaintext) const {
    std::string ciphertext = "";
    
    for (char c : plaintext) {
        if (std::isalpha(c)) {
            bool isUpper = std::isupper(c);
            char upper = std::toupper(c);
            int index = upper - 'A';
            char encrypted = key[index];
            
            if (!isUpper) {
                encrypted = std::tolower(encrypted);
            }
            ciphertext += encrypted;
        } else {
            ciphertext += c;
        }
    }
    
    return ciphertext;
}

std::string MonoalphabeticCipher::decrypt(const std::string& ciphertext) const {
    std::string plaintext = "";
    
    // Create reverse mapping
    char reverseKey[26];
    for (int i = 0; i < 26; i++) {
        int keyIndex = key[i] - 'A';
        reverseKey[keyIndex] = 'A' + i;
    }
    
    for (char c : ciphertext) {
        if (std::isalpha(c)) {
            bool isUpper = std::isupper(c);
            char upper = std::toupper(c);
            int index = upper - 'A';
            char decrypted = reverseKey[index];
            
            if (!isUpper) {
                decrypted = std::tolower(decrypted);
            }
            plaintext += decrypted;
        } else {
            plaintext += c;
        }
    }
    
    return plaintext;
}

void MonoalphabeticCipher::displayKey() const {
    std::cout << "\nSubstitution Key:\n";
    std::cout << "Plain:  ";
    for (int i = 0; i < 26; i++) {
        std::cout << (char)('A' + i) << " ";
    }
    std::cout << "\nCipher: ";
    for (int i = 0; i < 26; i++) {
        std::cout << key[i] << " ";
    }
    std::cout << "\n\n";
}
