#include "frequency_analyzer.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

std::vector<std::string> FrequencyAnalyzer::split_into_groups(const std::string& ciphertext, int keyLength) {
    std::vector<std::string> groups(keyLength);
    
    for (size_t i = 0; i < ciphertext.length(); i++) {
        if (std::isalpha(ciphertext[i])) {
            groups[i % keyLength] += std::toupper(ciphertext[i]);
        }
    }
    
    return groups;
}

std::vector<double> FrequencyAnalyzer::frequency_analysis(const std::string& text) {
    std::vector<int> counts(26, 0);
    int total = 0;
    
    // Count letters
    for (char c : text) {
        if (std::isalpha(c)) {
            counts[std::toupper(c) - 'A']++;
            total++;
        }
    }
    
    // Calculate percentages
    std::vector<double> frequencies(26);
    for (int i = 0; i < 26; i++) {
        frequencies[i] = (total > 0) ? (100.0 * counts[i] / total) : 0.0;
    }
    
    return frequencies;
}

double FrequencyAnalyzer::calculate_chi_square(const std::vector<double>& observed, int shift) {
    double chiSquare = 0.0;
    
    for (int i = 0; i < 26; i++) {
        // Shift the English frequency
        int shiftedIndex = (i + shift) % 26;
        double expected = englishFreq[i];
        double obs = observed[shiftedIndex];
        
        if (expected > 0) {
            chiSquare += ((obs - expected) * (obs - expected)) / expected;
        }
    }
    
    return chiSquare;
}

int FrequencyAnalyzer::find_shift(const std::string& groupText) {
    if (groupText.empty()) return 0;
    
    // Get frequency distribution
    std::vector<double> freq = frequency_analysis(groupText);
    
    // Try all possible shifts and find the one with minimum chi-square
    int bestShift = 0;
    double minChiSquare = 1e9;
    
    for (int shift = 0; shift < 26; shift++) {
        double chiSquare = calculate_chi_square(freq, shift);
        if (chiSquare < minChiSquare) {
            minChiSquare = chiSquare;
            bestShift = shift;
        }
    }
    
    return bestShift;
}

std::string FrequencyAnalyzer::find_key(const std::string& ciphertext, int keyLength) {
    std::cout << "\n========== KEY RECOVERY ==========\n";
    std::cout << "Analyzing each group with key length = " << keyLength << "\n\n";
    
    // Split into groups
    auto groups = split_into_groups(ciphertext, keyLength);
    
    std::string key = "";
    
    std::cout << std::left << std::setw(8) << "Group" 
              << std::setw(12) << "Size"
              << std::setw(12) << "Best Shift"
              << std::setw(12) << "Key Letter"
              << "Chi-Square\n";
    std::cout << std::string(60, '-') << "\n";
    
    for (int i = 0; i < keyLength; i++) {
        int shift = find_shift(groups[i]);
        char keyLetter = 'A' + shift;
        key += keyLetter;
        
        // Calculate chi-square for display
        auto freq = frequency_analysis(groups[i]);
        double chiSq = calculate_chi_square(freq, shift);
        
        std::cout << std::left << std::setw(8) << i
                  << std::setw(12) << groups[i].length()
                  << std::setw(12) << shift
                  << std::setw(12) << keyLetter
                  << std::fixed << std::setprecision(2) << chiSq << "\n";
    }
    
    std::cout << "\n";
    return key;
}

void FrequencyAnalyzer::display_frequency_table(const std::vector<double>& freq, int groupNumber) {
    std::cout << "\n--- Group " << groupNumber << " Frequency Distribution ---\n";
    std::cout << "Letter  Frequency  Percentage\n";
    std::cout << std::string(35, '-') << "\n";
    
    for (int i = 0; i < 26; i++) {
        char letter = 'A' + i;
        std::cout << std::setw(6) << letter
                  << std::setw(12) << std::fixed << std::setprecision(2) << freq[i] << "%"
                  << "\n";
    }
    std::cout << "\n";
}

void FrequencyAnalyzer::display_all_frequency_tables(const std::string& ciphertext, int keyLength) {
    std::cout << "\n========== FREQUENCY ANALYSIS FOR EACH GROUP ==========\n";
    
    auto groups = split_into_groups(ciphertext, keyLength);
    
    for (int i = 0; i < keyLength; i++) {
        auto freq = frequency_analysis(groups[i]);
        
        std::cout << "\n========== Group " << i << " (Key position " << i << ") ==========\n";
        std::cout << "Size: " << groups[i].length() << " letters\n\n";
        
        // Create list of (letter, frequency) pairs and sort
        std::vector<std::pair<char, double>> letterFreqs;
        for (int j = 0; j < 26; j++) {
            letterFreqs.push_back({'A' + j, freq[j]});
        }
        
        std::sort(letterFreqs.begin(), letterFreqs.end(),
                 [](const std::pair<char, double>& a, const std::pair<char, double>& b) {
                     return a.second > b.second;
                 });
        
        std::cout << std::left << std::setw(8) << "Letter" 
                  << std::setw(15) << "Percentage"
                  << "Bar Graph\n";
        std::cout << std::string(50, '-') << "\n";
        
        for (const auto& pair : letterFreqs) {
            if (pair.second > 0.5) {  // Only show letters with significant frequency
                std::cout << std::left << std::setw(8) << pair.first
                          << std::setw(15) << std::fixed << std::setprecision(2) << pair.second << "%";
                
                // Bar graph
                int barLength = static_cast<int>(pair.second / 2);  // Scale down
                std::cout << std::string(barLength, '=') << "\n";
            }
        }
        std::cout << "\n";
    }
}
