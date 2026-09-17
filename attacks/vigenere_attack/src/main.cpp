#include "kasiski_analysis.h"
#include "frequency_analyzer.h"
#include "vigenere_cipher.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

// Function to read file content
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to write file content
void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    file << content;
    file.close();
}

// Verification function
bool verify(const std::string& originalCipher, const std::string& recoveredPlaintext, const std::string& recoveredKey) {
    std::cout << "\n========== VERIFICATION ==========\n";
    
    // Re-encrypt the recovered plaintext
    std::string reencrypted = VigenereCipher::vigenere_encrypt(recoveredPlaintext, recoveredKey);
    
    // Clean both for comparison
    std::string cleanOriginal = VigenereCipher::clean_ciphertext(originalCipher);
    std::string cleanReencrypted = VigenereCipher::clean_ciphertext(reencrypted);
    
    // Compare
    int matches = 0;
    int total = std::min(cleanOriginal.length(), cleanReencrypted.length());
    
    for (int i = 0; i < total; i++) {
        if (cleanOriginal[i] == cleanReencrypted[i]) {
            matches++;
        }
    }
    
    double accuracy = (total > 0) ? (100.0 * matches / total) : 0.0;
    
    std::cout << "Original ciphertext length: " << cleanOriginal.length() << "\n";
    std::cout << "Re-encrypted text length: " << cleanReencrypted.length() << "\n";
    std::cout << "Matching characters: " << matches << "/" << total << "\n";
    std::cout << "Accuracy: " << std::fixed << std::setprecision(2) << accuracy << "%\n";
    
    if (accuracy >= 99.5) {
        std::cout << "\n✓ SUCCESS! Verification passed!\n";
        std::cout << "The recovered key correctly decrypts the ciphertext.\n";
        return true;
    } else if (accuracy >= 90.0) {
        std::cout << "\n⚠ PARTIAL SUCCESS: Key is mostly correct but may need refinement.\n";
        return false;
    } else {
        std::cout << "\n✗ VERIFICATION FAILED: Key is incorrect.\n";
        return false;
    }
}

// Main cryptanalysis function
void performCryptanalysis(const std::string& ciphertext, bool saveResults = true) {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "  VIGENÈRE CIPHER CRYPTANALYSIS\n";
    std::cout << "  Using Kasiski Examination and Frequency Analysis\n";
    std::cout << std::string(70, '=') << "\n";
    
    // Step 1: Preprocess ciphertext
    std::cout << "\n[Step 1] Preprocessing ciphertext...\n";
    std::string cleanCipher = VigenereCipher::clean_ciphertext(ciphertext);
    std::cout << "Original length: " << ciphertext.length() << " characters\n";
    std::cout << "Cleaned length: " << cleanCipher.length() << " characters\n";
    std::cout << "First 80 chars: " << cleanCipher.substr(0, 80) << "...\n";
    
    // Step 2: Kasiski Examination
    std::cout << "\n[Step 2] Performing Kasiski Examination...\n";
    KasiskiAnalyzer kasiskiAnalyzer(cleanCipher);
    auto kasiskiCandidates = kasiskiAnalyzer.kasiski_analysis();
    
    // Step 3: Index of Coincidence
    std::cout << "\n[Step 3] Calculating Index of Coincidence...\n";
    auto icCandidates = kasiskiAnalyzer.estimate_key_length_by_ic(20);
    
    // Combine candidates
    std::set<int> allCandidates;
    allCandidates.insert(kasiskiCandidates.begin(), kasiskiCandidates.end());
    allCandidates.insert(icCandidates.begin(), icCandidates.end());
    
    // Remove candidates < 2
    allCandidates.erase(1);
    
    if (allCandidates.empty()) {
        std::cout << "\n⚠ Warning: No key length candidates found. Trying default range...\n";
        for (int i = 3; i <= 10; i++) {
            allCandidates.insert(i);
        }
    }
    
    std::cout << "\n[Step 4] Combined key length candidates: ";
    for (int candidate : allCandidates) {
        std::cout << candidate << " ";
    }
    std::cout << "\n";
    
    // Try each candidate and find best one
    FrequencyAnalyzer freqAnalyzer;
    
    struct Result {
        int keyLength;
        std::string key;
        std::string plaintext;
        double score;
    };
    
    std::vector<Result> results;
    
    std::cout << "\n[Step 5] Trying each candidate key length...\n";
    
    for (int keyLen : allCandidates) {
        if (keyLen > 20) continue;  // Skip very long keys
        
        std::cout << "\n--- Testing key length = " << keyLen << " ---\n";
        
        // Find key
        std::string key = freqAnalyzer.find_key(cleanCipher, keyLen);
        std::cout << "Recovered key: " << key << "\n";
        
        // Decrypt
        std::string plaintext = VigenereCipher::vigenere_decrypt(cleanCipher, key);
        
        // Score the plaintext (count common English words/patterns)
        double score = kasiskiAnalyzer.calculate_ic(plaintext);
        
        std::cout << "IC of plaintext: " << std::fixed << std::setprecision(4) << score << "\n";
        std::cout << "First 100 chars: " << plaintext.substr(0, 100) << "...\n";
        
        results.push_back({keyLen, key, plaintext, score});
    }
    
    // Sort by IC score (closer to 0.0667 is better)
    std::sort(results.begin(), results.end(),
             [](const Result& a, const Result& b) {
                 double targetIC = 0.0667;
                 return std::abs(a.score - targetIC) < std::abs(b.score - targetIC);
             });
    
    // Display best result
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "  BEST RESULT\n";
    std::cout << std::string(70, '=') << "\n";
    
    Result best = results[0];
    
    std::cout << "\n✓ Estimated Key Length: " << best.keyLength << "\n";
    std::cout << "✓ Recovered Key: " << best.key << "\n";
    std::cout << "✓ IC Score: " << std::fixed << std::setprecision(4) << best.score << "\n";
    
    // Display frequency tables for best result
    std::cout << "\n[Step 6] Frequency Analysis for Key Length = " << best.keyLength << "\n";
    freqAnalyzer.display_all_frequency_tables(cleanCipher, best.keyLength);
    
    // Display recovered plaintext
    std::cout << "\n========== RECOVERED PLAINTEXT ==========\n";
    std::cout << best.plaintext << "\n";
    std::cout << "\n(Total length: " << best.plaintext.length() << " characters)\n";
    
    // Step 7: Verify
    std::cout << "\n[Step 7] Verification\n";
    bool verified = verify(cleanCipher, best.plaintext, best.key);
    
    // Save results
    if (saveResults) {
        writeFile("../outputs/recovered_key.txt", best.key);
        writeFile("../outputs/recovered_plaintext.txt", best.plaintext);
        writeFile("../outputs/cleaned_ciphertext.txt", cleanCipher);
        
        std::ofstream report("../outputs/analysis_report.txt");
        report << "VIGENÈRE CIPHER CRYPTANALYSIS REPORT\n";
        report << "=====================================\n\n";
        report << "Estimated Key Length: " << best.keyLength << "\n";
        report << "Recovered Key: " << best.key << "\n";
        report << "IC Score: " << best.score << "\n";
        report << "Verification: " << (verified ? "PASSED" : "FAILED") << "\n\n";
        report << "Recovered Plaintext:\n";
        report << best.plaintext << "\n";
        report.close();
        
        std::cout << "\n✓ Results saved to outputs/ directory\n";
    }
    
    // Show alternative candidates if verification failed
    if (!verified && results.size() > 1) {
        std::cout << "\n--- Alternative Candidates ---\n";
        for (size_t i = 1; i < std::min((size_t)3, results.size()); i++) {
            std::cout << "\nCandidate #" << (i+1) << ":\n";
            std::cout << "Key Length: " << results[i].keyLength << "\n";
            std::cout << "Key: " << results[i].key << "\n";
            std::cout << "IC: " << results[i].score << "\n";
            std::cout << "Preview: " << results[i].plaintext.substr(0, 80) << "...\n";
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Vigenère Cipher Cryptanalysis Tool\n";
    std::cout << "===================================\n\n";
    
    try {
        if (argc > 1) {
            // Read ciphertext from file
            std::string filename = argv[1];
            std::cout << "Reading ciphertext from: " << filename << "\n";
            std::string ciphertext = readFile(filename);
            performCryptanalysis(ciphertext);
        } else {
            // Interactive mode
            std::cout << "1. Analyze ciphertext from file\n";
            std::cout << "2. Analyze Ciphertext 1 (Odd groups)\n";
            std::cout << "3. Analyze Ciphertext 2 (Even groups)\n";
            std::cout << "4. Enter ciphertext manually\n";
            std::cout << "\nChoice: ";
            
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            
            std::string ciphertext;
            
            switch (choice) {
                case 1: {
                    std::cout << "Enter filename: ";
                    std::string filename;
                    std::getline(std::cin, filename);
                    ciphertext = readFile(filename);
                    break;
                }
                    
                case 2:
                    ciphertext = readFile("../testcases/ciphertext1_odd.txt");
                    break;
                    
                case 3:
                    ciphertext = readFile("../testcases/ciphertext2_even.txt");
                    break;
                    
                case 4: {
                    std::cout << "Enter ciphertext (Ctrl+D when done):\n";
                    std::string line;
                    while (std::getline(std::cin, line)) {
                        ciphertext += line + "\n";
                    }
                    break;
                }
                    
                default:
                    std::cout << "Invalid choice.\n";
                    return 1;
            }
            
            performCryptanalysis(ciphertext);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
