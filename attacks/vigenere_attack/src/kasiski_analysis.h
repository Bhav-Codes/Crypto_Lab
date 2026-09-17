#ifndef KASISKI_ANALYSIS_H
#define KASISKI_ANALYSIS_H

#include <string>
#include <vector>
#include <map>

// Structure to hold repeated pattern information
struct RepeatedPattern {
    std::string pattern;
    std::vector<int> positions;
    std::vector<int> distances;
    int frequency;
};

// Structure to hold factor analysis
struct FactorAnalysis {
    int factor;
    int frequency;
};

class KasiskiAnalyzer {
private:
    std::string ciphertext;
    int minPatternLength;
    int maxPatternLength;
    
    // English letter frequency (for comparison)
    const std::vector<double> englishFreq = {
        8.17, 1.49, 2.78, 4.25, 12.70, 2.23, 2.02, 6.09, 6.97, 0.15,
        0.77, 4.03, 2.41, 6.75, 7.51, 1.93, 0.10, 5.99, 6.33, 9.06,
        2.76, 0.98, 2.36, 0.15, 1.97, 0.07
    };

public:
    KasiskiAnalyzer(const std::string& cipher, int minLen = 3, int maxLen = 5);
    
    // Core analysis functions
    std::vector<RepeatedPattern> find_repeated_patterns();
    std::vector<int> calculate_distances(const std::vector<int>& positions);
    std::map<int, int> find_factors(const std::vector<int>& distances);
    std::vector<int> kasiski_analysis();
    
    // Index of Coincidence
    double calculate_ic(const std::string& text);
    std::vector<int> estimate_key_length_by_ic(int maxKeyLength = 20);
    
    // Helper functions
    void display_repeated_patterns(const std::vector<RepeatedPattern>& patterns);
    void display_factor_analysis(const std::map<int, int>& factors);
    std::vector<int> get_gcd_list(const std::vector<int>& distances);
};

#endif
