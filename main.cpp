/**
 * CryptoLabX - A Cryptanalysis Toolkit
 * Main entry point for the application
 */

#include <iostream>
#include <string>
#include <limits>
#include "utils/logger.h"
#include "utils/file_analyzer.h"

class CryptoLabX {
private:
    Logger logger;
    FileAnalyzer fileAnalyzer;
    bool running;

public:
    CryptoLabX() : running(true) {}

    void displayMenu() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "        CryptoLabX - Cryptanalysis Toolkit" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Encrypt" << std::endl;
        std::cout << "2. Decrypt" << std::endl;
        std::cout << "3. Attack" << std::endl;
        std::cout << "4. Analyze" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
    }

    void handleEncrypt() {
        std::cout << "\n[Encrypt] Coming Soon..." << std::endl;
        logger.log("Encrypt");
    }

    void handleDecrypt() {
        std::cout << "\n[Decrypt] Coming Soon..." << std::endl;
        logger.log("Decrypt");
    }

    void handleAttack() {
        std::cout << "\n[Attack] Coming Soon..." << std::endl;
        logger.log("Attack");
    }

    void handleAnalyze() {
        std::cout << "\n[Analyze] File Analysis" << std::endl;
        logger.log("Analyze");

        std::vector<std::string> files = fileAnalyzer.listDatasetFiles();

        if (files.empty()) {
            std::cout << "No text files found in datasets folder!" << std::endl;
            return;
        }

        std::cout << "\nAvailable files:" << std::endl;
        for (size_t i = 0; i < files.size(); i++) {
            std::cout << i + 1 << ". " << files[i] << std::endl;
        }

        std::cout << "\nSelect a file number (or press 0 to cancel): ";
        int choice;
        std::cin >> choice;

        // Clear input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice > 0 && choice <= static_cast<int>(files.size())) {
            std::string filePath = "datasets/" + files[choice - 1];
            fileAnalyzer.analyzeFile(filePath);
        } else if (choice != 0) {
            std::cout << "Invalid selection!" << std::endl;
        }
    }

    void handleExit() {
        std::cout << "\nThank you for using CryptoLabX!" << std::endl;
        logger.log("Exit");
        running = false;
    }

    void run() {
        std::cout << "Welcome to CryptoLabX!" << std::endl;

        while (running) {
            displayMenu();
            std::cout << "\nEnter your choice (1-5): ";
            
            int choice;
            std::cin >> choice;

            // Handle invalid input
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nInvalid choice! Please select 1-5." << std::endl;
                continue;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            switch (choice) {
                case 1:
                    handleEncrypt();
                    break;
                case 2:
                    handleDecrypt();
                    break;
                case 3:
                    handleAttack();
                    break;
                case 4:
                    handleAnalyze();
                    break;
                case 5:
                    handleExit();
                    break;
                default:
                    std::cout << "\nInvalid choice! Please select 1-5." << std::endl;
            }
        }
    }
};

int main() {
    try {
        CryptoLabX app;
        app.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nAn error occurred: " << e.what() << std::endl;
        return 1;
    }
}
