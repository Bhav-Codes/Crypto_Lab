#include "cryptanalysis.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>
#include <set>

CryptanalysisEngine::CryptanalysisEngine(const std::string& cipher) 
    : ciphertext(cipher), partialPlaintext(cipher) {
    // Initialize partial plaintext with underscores for letters
    for (size_t i = 0; i < partialPlaintext.length(); i++) {
        if (std::isalpha(partialPlaintext[i])) {
            partialPlaintext[i] = '_';
        }
    }
}

std::string CryptanalysisEngine::extractLettersOnly(const std::string& text) const {
    std::string result = "";
    for (char c : text) {
        if (std::isalpha(c)) {
            result += std::toupper(c);
        }
    }
    return result;
}

std::vector<LetterFrequency> CryptanalysisEngine::frequency_analysis() {
    std::map<char, int> frequency;
    int totalLetters = 0;
    
    // Count frequency of each letter
    for (char c : ciphertext) {
        if (std::isalpha(c)) {
            char upper = std::toupper(c);
            frequency[upper]++;
            totalLetters++;
        }
    }
    
    // Convert to vector and calculate percentages
    std::vector<LetterFrequency> result;
    for (const auto& pair : frequency) {
        LetterFrequency lf;
        lf.letter = pair.first;
        lf.count = pair.second;
        lf.percentage = (totalLetters > 0) ? (100.0 * pair.second / totalLetters) : 0.0;
        result.push_back(lf);
    }
    
    // Sort by frequency (descending)
    std::sort(result.begin(), result.end(), 
              [](const LetterFrequency& a, const LetterFrequency& b) {
                  return a.count > b.count;
              });
    
    // Display results
    std::cout << "\n========== LETTER FREQUENCY ANALYSIS ==========\n";
    std::cout << std::left << std::setw(8) << "Letter" 
              << std::setw(10) << "Count" 
              << std::setw(12) << "Percentage"
              << "Expected (English)\n";
    std::cout << std::string(50, '-') << "\n";
    
    for (const auto& lf : result) {
        double expected = 0.0;
        auto it = englishFrequency.find(lf.letter);
        if (it != englishFrequency.end()) {
            expected = it->second;
        }
        
        std::cout << std::left << std::setw(8) << lf.letter
                  << std::setw(10) << lf.count
                  << std::setw(12) << std::fixed << std::setprecision(2) << lf.percentage << "%"
                  << std::fixed << std::setprecision(2) << expected << "%\n";
    }
    std::cout << "\nTotal letters analyzed: " << totalLetters << "\n\n";
    
    return result;
}

std::map<int, std::vector<std::string>> CryptanalysisEngine::word_frequency_analysis() {
    std::map<int, std::map<std::string, int>> wordsByLength;
    std::string currentWord = "";
    
    // Extract words
    for (char c : ciphertext) {
        if (std::isalpha(c)) {
            currentWord += std::toupper(c);
        } else if (!currentWord.empty()) {
            int len = currentWord.length();
            wordsByLength[len][currentWord]++;
            currentWord = "";
        }
    }
    if (!currentWord.empty()) {
        int len = currentWord.length();
        wordsByLength[len][currentWord]++;
    }
    
    // Organize by frequency
    std::map<int, std::vector<std::string>> result;
    
    std::cout << "\n========== WORD FREQUENCY ANALYSIS ==========\n";
    
    for (int len = 1; len <= 3; len++) {
        if (wordsByLength.find(len) != wordsByLength.end()) {
            std::vector<std::pair<std::string, int>> words;
            for (const auto& pair : wordsByLength[len]) {
                words.push_back(pair);
            }
            
            // Sort by frequency
            std::sort(words.begin(), words.end(),
                     [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                         return a.second > b.second;
                     });
            
            std::cout << "\n" << len << "-letter words:\n";
            int count = 0;
            for (const auto& pair : words) {
                result[len].push_back(pair.first);
                std::cout << "  " << pair.first << " (" << pair.second << " times)";
                count++;
                if (count % 5 == 0) std::cout << "\n";
                if (count >= 20) break; // Show top 20
            }
            if (count % 5 != 0) std::cout << "\n";
        }
    }
    
    // Show repeated words
    std::cout << "\nRepeated words (4+ letters):\n";
    std::vector<std::pair<std::string, int>> allWords;
    for (const auto& lenPair : wordsByLength) {
        if (lenPair.first >= 4) {
            for (const auto& wordPair : lenPair.second) {
                if (wordPair.second >= 2) {
                    allWords.push_back(wordPair);
                }
            }
        }
    }
    
    std::sort(allWords.begin(), allWords.end(),
             [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                 return a.second > b.second;
             });
    
    int count = 0;
    for (const auto& pair : allWords) {
        std::cout << "  " << pair.first << " (" << pair.second << " times)\n";
        count++;
        if (count >= 15) break;
    }
    std::cout << "\n";
    
    return result;
}

std::string CryptanalysisEngine::getWordPattern(const std::string& word) const {
    std::map<char, char> charMap;
    std::string pattern = "";
    char nextPattern = '1';
    
    for (char c : word) {
        if (charMap.find(c) == charMap.end()) {
            charMap[c] = nextPattern++;
        }
        pattern += charMap[c];
    }
    
    return pattern;
}

std::vector<WordPattern> CryptanalysisEngine::pattern_analysis() {
    std::map<std::string, WordPattern> patterns;
    std::string currentWord = "";
    
    // Extract words and their patterns
    for (char c : ciphertext) {
        if (std::isalpha(c)) {
            currentWord += std::toupper(c);
        } else if (!currentWord.empty()) {
            std::string pattern = getWordPattern(currentWord);
            if (patterns.find(pattern) == patterns.end()) {
                WordPattern wp;
                wp.word = currentWord;
                wp.pattern = pattern;
                wp.frequency = 1;
                patterns[pattern] = wp;
            } else {
                patterns[pattern].frequency++;
            }
            currentWord = "";
        }
    }
    if (!currentWord.empty()) {
        std::string pattern = getWordPattern(currentWord);
        if (patterns.find(pattern) == patterns.end()) {
            WordPattern wp;
            wp.word = currentWord;
            wp.pattern = pattern;
            wp.frequency = 1;
            patterns[pattern] = wp;
        } else {
            patterns[pattern].frequency++;
        }
    }
    
    // Convert to vector and sort
    std::vector<WordPattern> result;
    for (const auto& pair : patterns) {
        result.push_back(pair.second);
    }
    
    std::sort(result.begin(), result.end(),
             [](const WordPattern& a, const WordPattern& b) {
                 if (a.frequency != b.frequency) return a.frequency > b.frequency;
                 return a.word.length() > b.word.length();
             });
    
    // Display patterns
    std::cout << "\n========== PATTERN ANALYSIS ==========\n";
    std::cout << "Repeated letter patterns (useful for cryptanalysis):\n\n";
    
    int shown = 0;
    for (const auto& wp : result) {
        if (wp.frequency >= 2 && wp.word.length() >= 3) {
            std::cout << "Pattern: " << wp.pattern << " - Example: " << wp.word 
                     << " (" << wp.frequency << " occurrences)\n";
            shown++;
            if (shown >= 20) break;
        }
    }
    std::cout << "\n";
    
    return result;
}

bool CryptanalysisEngine::apply_substitution(char cipherChar, char plainChar) {
    cipherChar = std::toupper(cipherChar);
    plainChar = std::toupper(plainChar);
    
    // Check if this substitution conflicts with existing ones
    if (currentSubstitution.find(cipherChar) != currentSubstitution.end()) {
        if (currentSubstitution[cipherChar] != plainChar) {
            std::cout << "Warning: " << cipherChar << " is already mapped to " 
                     << currentSubstitution[cipherChar] << "\n";
            return false;
        }
    }
    
    if (reverseSubstitution.find(plainChar) != reverseSubstitution.end()) {
        if (reverseSubstitution[plainChar] != cipherChar) {
            std::cout << "Warning: " << plainChar << " is already mapped from " 
                     << reverseSubstitution[plainChar] << "\n";
            return false;
        }
    }
    
    // Apply substitution
    currentSubstitution[cipherChar] = plainChar;
    reverseSubstitution[plainChar] = cipherChar;
    
    // Update partial plaintext
    for (size_t i = 0; i < ciphertext.length(); i++) {
        if (std::isalpha(ciphertext[i])) {
            char upper = std::toupper(ciphertext[i]);
            if (currentSubstitution.find(upper) != currentSubstitution.end()) {
                char substituted = currentSubstitution[upper];
                partialPlaintext[i] = std::isupper(ciphertext[i]) ? substituted : std::tolower(substituted);
            }
        }
    }
    
    std::cout << "Applied substitution: " << cipherChar << " -> " << plainChar << "\n";
    return true;
}

void CryptanalysisEngine::remove_substitution(char cipherChar) {
    cipherChar = std::toupper(cipherChar);
    
    if (currentSubstitution.find(cipherChar) != currentSubstitution.end()) {
        char plainChar = currentSubstitution[cipherChar];
        currentSubstitution.erase(cipherChar);
        reverseSubstitution.erase(plainChar);
        
        // Reset partial plaintext for this cipher character
        for (size_t i = 0; i < ciphertext.length(); i++) {
            if (std::toupper(ciphertext[i]) == cipherChar) {
                partialPlaintext[i] = '_';
            }
        }
        
        std::cout << "Removed substitution: " << cipherChar << "\n";
    }
}

void CryptanalysisEngine::clear_all_substitutions() {
    currentSubstitution.clear();
    reverseSubstitution.clear();
    
    // Reset partial plaintext
    for (size_t i = 0; i < partialPlaintext.length(); i++) {
        if (std::isalpha(ciphertext[i])) {
            partialPlaintext[i] = '_';
        }
    }
    
    std::cout << "All substitutions cleared.\n";
}

void CryptanalysisEngine::display_partial_plaintext() {
    std::cout << "\n========== PARTIAL PLAINTEXT ==========\n";
    std::cout << "Ciphertext: " << ciphertext.substr(0, 80) << "...\n";
    std::cout << "Plaintext:  " << partialPlaintext.substr(0, 80) << "...\n\n";
    
    // Show progress
    int solved = 0;
    int total = 0;
    for (char c : ciphertext) {
        if (std::isalpha(c)) {
            total++;
            char upper = std::toupper(c);
            if (currentSubstitution.find(upper) != currentSubstitution.end()) {
                solved++;
            }
        }
    }
    
    double progress = (total > 0) ? (100.0 * solved / total) : 0.0;
    std::cout << "Progress: " << solved << "/" << total << " letters (" 
             << std::fixed << std::setprecision(1) << progress << "%)\n\n";
}

void CryptanalysisEngine::display_current_mapping() {
    std::cout << "\n========== CURRENT SUBSTITUTION MAPPING ==========\n";
    std::cout << "Cipher -> Plain:\n";
    
    for (char c = 'A'; c <= 'Z'; c++) {
        if (currentSubstitution.find(c) != currentSubstitution.end()) {
            std::cout << c << " -> " << currentSubstitution[c] << "   ";
        } else {
            std::cout << c << " -> _   ";
        }
        if ((c - 'A' + 1) % 7 == 0) std::cout << "\n";
    }
    std::cout << "\n\n";
}

bool CryptanalysisEngine::verify_solution(const std::string& originalPlaintext) {
    std::string decrypted = partialPlaintext;
    
    // Extract only letters for comparison
    std::string original = extractLettersOnly(originalPlaintext);
    std::string decryptedLetters = extractLettersOnly(decrypted);
    
    if (original.length() != decryptedLetters.length()) {
        std::cout << "Length mismatch in verification.\n";
        return false;
    }
    
    int matches = 0;
    for (size_t i = 0; i < original.length(); i++) {
        if (original[i] == decryptedLetters[i]) {
            matches++;
        }
    }
    
    double accuracy = (original.length() > 0) ? (100.0 * matches / original.length()) : 0.0;
    
    std::cout << "\n========== VERIFICATION RESULTS ==========\n";
    std::cout << "Matches: " << matches << "/" << original.length() 
             << " (" << std::fixed << std::setprecision(2) << accuracy << "%)\n";
    
    if (accuracy >= 99.5) {
        std::cout << "SUCCESS! Plaintext successfully recovered!\n\n";
        return true;
    } else {
        std::cout << "Partial solution. Continue analysis.\n\n";
        return false;
    }
}

std::string CryptanalysisEngine::get_partial_plaintext() const {
    return partialPlaintext;
}

std::map<char, char> CryptanalysisEngine::get_current_substitution() const {
    return currentSubstitution;
}

void CryptanalysisEngine::save_analysis_to_file(const std::string& filename) {
    std::ofstream outFile(filename);
    
    if (!outFile.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return;
    }
    
    outFile << "CRYPTANALYSIS REPORT\n";
    outFile << "====================\n\n";
    
    outFile << "Current Substitution Mapping:\n";
    for (char c = 'A'; c <= 'Z'; c++) {
        if (currentSubstitution.find(c) != currentSubstitution.end()) {
            outFile << c << " -> " << currentSubstitution.at(c) << "\n";
        }
    }
    
    outFile << "\nPartial Plaintext:\n";
    outFile << partialPlaintext << "\n";
    
    outFile.close();
    std::cout << "Analysis saved to: " << filename << "\n";
}
