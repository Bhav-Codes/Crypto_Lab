#ifndef MONOALPHABETIC_CIPHER_H
#define MONOALPHABETIC_CIPHER_H

#include <string>
#include <iostream>

class MonoalphabeticCipher {
private:
    char key[26]; // Substitution key mapping A-Z

public:
    MonoalphabeticCipher();
    
    void generateRandomKey();
    void setKey(const std::string& keyStr);
    std::string getKey() const;
    
    std::string encrypt(const std::string& plaintext) const;
    std::string decrypt(const std::string& ciphertext) const;
    
    void displayKey() const;
};

#endif
