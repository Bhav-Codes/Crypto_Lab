#ifndef CRYPTANALYSIS_H
#define CRYPTANALYSIS_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <iomanip>

struct LetterFrequency {
    char letter;
    int count;
    double percentage;
};

struct WordPattern {
    std::string word;
    std::string pattern;
    int frequency;
};

class CryptanalysisEngine {
private:
    std::string ciphertext;
    std::string partialPlaintext;
    std::map<char, char> currentSubstitution; // cipher -> plain mapping
    std::map<char, char> reverseSubstitution; // plain -> cipher mapping
    
    // English letter frequency (approximate)
    const std::map<char, double> englishFrequency = {
        {'E', 12.70}, {'T', 9.06}, {'A', 8.17}, {'O', 7.51}, {'I', 6.97},
        {'N', 6.75}, {'S', 6.33}, {'H', 6.09}, {'R', 5.99}, {'D', 4.25},
        {'L', 4.03}, {'C', 2.78}, {'U', 2.76}, {'M', 2.41}, {'W', 2.36},
        {'F', 2.23}, {'G', 2.02}, {'Y', 1.97}, {'P', 1.93}, {'B', 1.29},
        {'V', 0.98}, {'K', 0.77}, {'J', 0.15}, {'X', 0.15}, {'Q', 0.10},
        {'Z', 0.07}
    };
    
    std::string extractLettersOnly(const std::string& text) const;
    std::string getWordPattern(const std::string& word) const;

public:
    CryptanalysisEngine(const std::string& cipher);
    
    // Main analysis functions
    std::vector<LetterFrequency> frequency_analysis();
    std::map<int, std::vector<std::string>> word_frequency_analysis();
    std::vector<WordPattern> pattern_analysis();
    
    // Substitution operations
    bool apply_substitution(char cipherChar, char plainChar);
    void remove_substitution(char cipherChar);
    void clear_all_substitutions();
    
    // Display and verification
    void display_partial_plaintext();
    void display_current_mapping();
    bool verify_solution(const std::string& originalPlaintext);
    
    // Utility functions
    std::string get_partial_plaintext() const;
    std::map<char, char> get_current_substitution() const;
    void save_analysis_to_file(const std::string& filename);
};

#endif
