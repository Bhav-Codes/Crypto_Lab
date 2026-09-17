#include "kasiski_analysis.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

KasiskiAnalyzer::KasiskiAnalyzer(const std::string& cipher, int minLen, int maxLen) 
    : ciphertext(cipher), minPatternLength(minLen), maxPatternLength(maxLen) {
}

std::vector<RepeatedPattern> KasiskiAnalyzer::find_repeated_patterns() {
    std::map<std::string, std::vector<int>> patternPositions;
    
    // Find all patterns of various lengths
    for (int len = minPatternLength; len <= maxPatternLength; len++) {
        for (size_t i = 0; i <= ciphertext.length() - len; i++) {
            std::string pattern = ciphertext.substr(i, len);
            patternPositions[pattern].push_back(i);
        }
    }
    
    // Filter patterns that appear more than once
    std::vector<RepeatedPattern> repeatedPatterns;
    for (const auto& pair : patternPositions) {
        if (pair.second.size() > 1) {
            RepeatedPattern rp;
            rp.pattern = pair.first;
            rp.positions = pair.second;
            rp.distances = calculate_distances(pair.second);
            rp.frequency = pair.second.size();
            repeatedPatterns.push_back(rp);
        }
    }
    
    // Sort by frequency (descending) and then by pattern length
    std::sort(repeatedPatterns.begin(), repeatedPatterns.end(),
             [](const RepeatedPattern& a, const RepeatedPattern& b) {
                 if (a.frequency != b.frequency) return a.frequency > b.frequency;
                 return a.pattern.length() > b.pattern.length();
             });
    
    return repeatedPatterns;
}

std::vector<int> KasiskiAnalyzer::calculate_distances(const std::vector<int>& positions) {
    std::vector<int> distances;
    for (size_t i = 1; i < positions.size(); i++) {
        distances.push_back(positions[i] - positions[i-1]);
    }
    return distances;
}

std::map<int, int> KasiskiAnalyzer::find_factors(const std::vector<int>& distances) {
    std::map<int, int> factorCount;
    
    for (int distance : distances) {
        // Find all factors of this distance
        for (int factor = 2; factor <= distance / 2; factor++) {
            if (distance % factor == 0) {
                factorCount[factor]++;
            }
        }
        // The distance itself is also a factor
        if (distance > 1) {
            factorCount[distance]++;
        }
    }
    
    return factorCount;
}

std::vector<int> KasiskiAnalyzer::kasiski_analysis() {
    std::cout << "\n========== KASISKI EXAMINATION ==========\n";
    
    // Step 1: Find repeated patterns
    auto patterns = find_repeated_patterns();
    
    std::cout << "\nStep 1: Finding repeated patterns...\n";
    std::cout << "Found " << patterns.size() << " repeated patterns\n\n";
    
    // Display top patterns
    std::cout << "Top repeated patterns:\n";
    std::cout << std::left << std::setw(10) << "Pattern" 
              << std::setw(12) << "Frequency" 
              << std::setw(15) << "Distances"
              << "Positions\n";
    std::cout << std::string(70, '-') << "\n";
    
    int displayCount = 0;
    for (const auto& rp : patterns) {
        if (displayCount++ >= 15) break;
        
        std::cout << std::left << std::setw(10) << rp.pattern
                  << std::setw(12) << rp.frequency;
        
        // Display distances
        std::cout << std::setw(15);
        for (size_t i = 0; i < rp.distances.size() && i < 3; i++) {
            std::cout << rp.distances[i];
            if (i < rp.distances.size() - 1 && i < 2) std::cout << ",";
        }
        
        // Display first few positions
        std::cout << "  ";
        for (size_t i = 0; i < rp.positions.size() && i < 3; i++) {
            std::cout << rp.positions[i];
            if (i < rp.positions.size() - 1 && i < 2) std::cout << ",";
        }
        std::cout << "\n";
    }
    
    // Step 2: Calculate all distances
    std::vector<int> allDistances;
    for (const auto& rp : patterns) {
        allDistances.insert(allDistances.end(), rp.distances.begin(), rp.distances.end());
    }
    
    std::cout << "\nStep 2: Collected " << allDistances.size() << " distances\n";
    
    // Step 3: Find factors
    auto factorFrequency = find_factors(allDistances);
    
    std::cout << "\nStep 3: Factor analysis of distances\n";
    std::cout << std::left << std::setw(15) << "Key Length" 
              << "Frequency (votes)\n";
    std::cout << std::string(40, '-') << "\n";
    
    // Convert to vector and sort by frequency
    std::vector<std::pair<int, int>> sortedFactors(factorFrequency.begin(), factorFrequency.end());
    std::sort(sortedFactors.begin(), sortedFactors.end(),
             [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                 return a.second > b.second;
             });
    
    // Display top factors
    std::vector<int> candidateKeyLengths;
    for (size_t i = 0; i < sortedFactors.size() && i < 15; i++) {
        std::cout << std::left << std::setw(15) << sortedFactors[i].first
                  << sortedFactors[i].second << "\n";
        if (i < 5) {
            candidateKeyLengths.push_back(sortedFactors[i].first);
        }
    }
    
    std::cout << "\n";
    return candidateKeyLengths;
}

double KasiskiAnalyzer::calculate_ic(const std::string& text) {
    if (text.length() < 2) return 0.0;
    
    // Count frequency of each letter
    std::vector<int> freq(26, 0);
    int totalLetters = 0;
    
    for (char c : text) {
        if (std::isalpha(c)) {
            freq[std::toupper(c) - 'A']++;
            totalLetters++;
        }
    }
    
    if (totalLetters < 2) return 0.0;
    
    // Calculate IC: sum of ni(ni-1) / N(N-1)
    double ic = 0.0;
    for (int count : freq) {
        ic += count * (count - 1);
    }
    ic /= (totalLetters * (totalLetters - 1));
    
    return ic;
}

std::vector<int> KasiskiAnalyzer::estimate_key_length_by_ic(int maxKeyLength) {
    std::cout << "\n========== INDEX OF COINCIDENCE ANALYSIS ==========\n";
    std::cout << "Expected IC for English: ~0.0667\n";
    std::cout << "Expected IC for random text: ~0.0385\n\n";
    
    std::vector<std::pair<int, double>> keyLengthScores;
    
    std::cout << std::left << std::setw(15) << "Key Length" 
              << std::setw(15) << "Avg IC"
              << "Verdict\n";
    std::cout << std::string(50, '-') << "\n";
    
    for (int keyLen = 1; keyLen <= maxKeyLength; keyLen++) {
        // Split text into groups
        std::vector<std::string> groups(keyLen);
        for (size_t i = 0; i < ciphertext.length(); i++) {
            groups[i % keyLen] += ciphertext[i];
        }
        
        // Calculate average IC across all groups
        double avgIC = 0.0;
        for (const auto& group : groups) {
            avgIC += calculate_ic(group);
        }
        avgIC /= keyLen;
        
        keyLengthScores.push_back({keyLen, avgIC});
        
        std::string verdict = "";
        if (avgIC > 0.060) {
            verdict = "<-- Likely!";
        } else if (avgIC > 0.045) {
            verdict = "<-- Possible";
        }
        
        std::cout << std::left << std::setw(15) << keyLen
                  << std::setw(15) << std::fixed << std::setprecision(4) << avgIC
                  << verdict << "\n";
    }
    
    // Sort by IC (closer to English ~0.0667)
    std::sort(keyLengthScores.begin(), keyLengthScores.end(),
             [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
                 double targetIC = 0.0667;
                 return std::abs(a.second - targetIC) < std::abs(b.second - targetIC);
             });
    
    std::vector<int> candidates;
    for (size_t i = 0; i < 5 && i < keyLengthScores.size(); i++) {
        if (keyLengthScores[i].second > 0.045) {
            candidates.push_back(keyLengthScores[i].first);
        }
    }
    
    std::cout << "\n";
    return candidates;
}

void KasiskiAnalyzer::display_repeated_patterns(const std::vector<RepeatedPattern>& patterns) {
    display_factor_analysis(find_factors({}));
}

void KasiskiAnalyzer::display_factor_analysis(const std::map<int, int>& factors) {
    // Implementation already in kasiski_analysis()
}

std::vector<int> KasiskiAnalyzer::get_gcd_list(const std::vector<int>& distances) {
    std::vector<int> result;
    // Implementation for GCD computation if needed
    return result;
}
