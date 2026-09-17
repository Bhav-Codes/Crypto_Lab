#ifndef FREQUENCY_ANALYZER_H
#define FREQUENCY_ANALYZER_H

#include <string>
#include <vector>
#include <map>

class FrequencyAnalyzer {
private:
    // English letter frequency
    const std::vector<double> englishFreq = {
        8.17, 1.49, 2.78, 4.25, 12.70, 2.23, 2.02, 6.09, 6.97, 0.15,
        0.77, 4.03, 2.41, 6.75, 7.51, 1.93, 0.10, 5.99, 6.33, 9.06,
        2.76, 0.98, 2.36, 0.15, 1.97, 0.07
    };

public:
    // Core frequency analysis functions
    std::vector<std::string> split_into_groups(const std::string& ciphertext, int keyLength);
    std::vector<double> frequency_analysis(const std::string& text);
    int find_shift(const std::string& groupText);
    std::string find_key(const std::string& ciphertext, int keyLength);
    
    // Helper functions
    double calculate_chi_square(const std::vector<double>& observed, int shift);
    void display_frequency_table(const std::vector<double>& freq, int groupNumber);
    void display_all_frequency_tables(const std::string& ciphertext, int keyLength);
};

#endif
