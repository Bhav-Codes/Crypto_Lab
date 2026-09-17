#include "monoalphabetic_cipher.h"
#include "cryptanalysis.h"
#include <fstream>
#include <sstream>
#include <limits>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    file << content;
    file.close();
}

void generateCiphertext(const std::string& plaintextFile, const std::string& ciphertextFile, const std::string& keyFile) {
    std::cout << "\n========== ENCRYPTION MODE ==========\n";
    
    // Read plaintext
    std::string plaintext = readFile(plaintextFile);
    std::cout << "Plaintext loaded: " << plaintext.length() << " characters\n";
    
    // Generate cipher
    MonoalphabeticCipher cipher;
    cipher.generateRandomKey();
    
    std::cout << "\nGenerated substitution key:\n";
    cipher.displayKey();
    
    // Encrypt
    std::string ciphertext = cipher.encrypt(plaintext);
    
    // Save outputs
    writeFile(ciphertextFile, ciphertext);
    writeFile(keyFile, cipher.getKey());
    
    std::cout << "Ciphertext saved to: " << ciphertextFile << "\n";
    std::cout << "Key saved to: " << keyFile << "\n";
    std::cout << "Ciphertext length: " << ciphertext.length() << " characters\n\n";
}

void interactiveCryptanalysis(const std::string& ciphertextFile, const std::string& plaintextFile) {
    std::cout << "\n========== CRYPTANALYSIS MODE ==========\n";
    
    // Read ciphertext
    std::string ciphertext = readFile(ciphertextFile);
    std::cout << "Ciphertext loaded: " << ciphertext.length() << " characters\n";
    
    // Read original plaintext for verification (optional)
    std::string originalPlaintext = "";
    try {
        originalPlaintext = readFile(plaintextFile);
        std::cout << "Original plaintext loaded for verification\n";
    } catch (...) {
        std::cout << "No plaintext file for verification\n";
    }
    
    // Create cryptanalysis engine
    CryptanalysisEngine engine(ciphertext);
    
    std::cout << "\n========== AUTOMATED ANALYSIS ==========\n";
    std::cout << "Running frequency analysis...\n";
    auto letterFreq = engine.frequency_analysis();
    
    std::cout << "\nRunning word frequency analysis...\n";
    auto wordFreq = engine.word_frequency_analysis();
    
    std::cout << "\nRunning pattern analysis...\n";
    auto patterns = engine.pattern_analysis();
    
    // Interactive menu
    bool running = true;
    int stepNumber = 1;
    
    std::cout << "\n========== INTERACTIVE CRYPTANALYSIS ==========\n";
    std::cout << "Use the analysis above to make substitution decisions.\n";
    std::cout << "Document each decision for your notebook.\n\n";
    
    while (running) {
        std::cout << "\n=== Step " << stepNumber << " ===\n";
        std::cout << "1. View partial plaintext\n";
        std::cout << "2. View current mapping\n";
        std::cout << "3. Add substitution\n";
        std::cout << "4. Remove substitution\n";
        std::cout << "5. Clear all substitutions\n";
        std::cout << "6. Re-run frequency analysis\n";
        std::cout << "7. Re-run word analysis\n";
        std::cout << "8. Re-run pattern analysis\n";
        std::cout << "9. Verify solution\n";
        std::cout << "10. Save progress\n";
        std::cout << "11. Auto-suggest (basic)\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again.\n";
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                engine.display_partial_plaintext();
                break;
                
            case 2:
                engine.display_current_mapping();
                break;
                
            case 3: {
                char cipherChar, plainChar;
                std::cout << "Enter cipher letter: ";
                std::cin >> cipherChar;
                std::cout << "Enter plain letter: ";
                std::cin >> plainChar;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (engine.apply_substitution(cipherChar, plainChar)) {
                    std::cout << "\nDocument this in your notebook:\n";
                    std::cout << "Step " << stepNumber << ": Applied " << (char)std::toupper(cipherChar) 
                             << " -> " << (char)std::toupper(plainChar) << "\n";
                    stepNumber++;
                }
                break;
            }
                
            case 4: {
                char cipherChar;
                std::cout << "Enter cipher letter to remove: ";
                std::cin >> cipherChar;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                engine.remove_substitution(cipherChar);
                break;
            }
                
            case 5:
                engine.clear_all_substitutions();
                stepNumber = 1;
                break;
                
            case 6:
                engine.frequency_analysis();
                break;
                
            case 7:
                engine.word_frequency_analysis();
                break;
                
            case 8:
                engine.pattern_analysis();
                break;
                
            case 9:
                if (!originalPlaintext.empty()) {
                    engine.verify_solution(originalPlaintext);
                } else {
                    std::cout << "No original plaintext available for verification.\n";
                }
                break;
                
            case 10: {
                std::string filename = "../outputs/cryptanalysis_progress.txt";
                engine.save_analysis_to_file(filename);
                break;
            }
                
            case 11: {
                std::cout << "\n=== AUTO-SUGGEST ===\n";
                std::cout << "Based on English frequency, suggested mappings:\n";
                if (letterFreq.size() >= 5) {
                    std::cout << letterFreq[0].letter << " -> E (most frequent)\n";
                    std::cout << letterFreq[1].letter << " -> T\n";
                    std::cout << letterFreq[2].letter << " -> A\n";
                    std::cout << letterFreq[3].letter << " -> O\n";
                    std::cout << letterFreq[4].letter << " -> I\n";
                }
                std::cout << "\nCommon one-letter words: A, I\n";
                std::cout << "Common two-letter words: OF, TO, IN, IT, IS, BE, AS, AT, SO, WE\n";
                std::cout << "Common three-letter words: THE, AND, FOR, ARE, BUT, NOT, YOU, ALL\n";
                std::cout << "\nUse these suggestions carefully with pattern analysis!\n";
                break;
            }
                
            case 0:
                running = false;
                std::cout << "Exiting cryptanalysis.\n";
                break;
                
            default:
                std::cout << "Invalid choice.\n";
        }
    }
}

void displayMenu() {
    std::cout << "\n========================================\n";
    std::cout << "MONOALPHABETIC CIPHER CRYPTANALYSIS\n";
    std::cout << "========================================\n";
    std::cout << "1. Generate ciphertext from plaintext\n";
    std::cout << "2. Perform cryptanalysis on ciphertext\n";
    std::cout << "3. Test with sample data\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

int main(int argc, char* argv[]) {
    std::cout << "Monoalphabetic Substitution Cipher - Cryptanalysis Tool\n";
    std::cout << "========================================================\n";
    
    bool running = true;
    
    while (running) {
        displayMenu();
        
        int choice;
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input.\n";
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        try {
            switch (choice) {
                case 1: {
                    std::string plaintextFile, ciphertextFile, keyFile;
                    std::cout << "Enter plaintext file: ";
                    std::getline(std::cin, plaintextFile);
                    std::cout << "Enter output ciphertext file: ";
                    std::getline(std::cin, ciphertextFile);
                    std::cout << "Enter output key file: ";
                    std::getline(std::cin, keyFile);
                    
                    generateCiphertext(plaintextFile, ciphertextFile, keyFile);
                    break;
                }
                    
                case 2: {
                    std::string ciphertextFile, plaintextFile;
                    std::cout << "Enter ciphertext file: ";
                    std::getline(std::cin, ciphertextFile);
                    std::cout << "Enter plaintext file (for verification, or leave empty): ";
                    std::getline(std::cin, plaintextFile);
                    
                    interactiveCryptanalysis(ciphertextFile, plaintextFile);
                    break;
                }
                    
                case 3: {
                    std::cout << "\nUsing sample test case...\n";
                    generateCiphertext(
                        "../testcases/sample_plaintext.txt",
                        "../outputs/sample_ciphertext.txt",
                        "../outputs/sample_key.txt"
                    );
                    
                    std::cout << "\nNow starting cryptanalysis...\n";
                    interactiveCryptanalysis(
                        "../outputs/sample_ciphertext.txt",
                        "../testcases/sample_plaintext.txt"
                    );
                    break;
                }
                    
                case 0:
                    running = false;
                    std::cout << "Goodbye!\n";
                    break;
                    
                default:
                    std::cout << "Invalid choice.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    
    return 0;
}
