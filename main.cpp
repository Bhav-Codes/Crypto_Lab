// CryptoLabX - A Cryptanalysis Toolkit
// Simple and basic C++ implementation

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

// Function to write to log file
void writeLog(string action) {
    ofstream logFile("outputs/activity.log", ios::app);
    if (logFile.is_open()) {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        logFile << "[" << 1900 + ltm->tm_year << "-" 
                << 1 + ltm->tm_mon << "-" << ltm->tm_mday << " "
                << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec
                << "] Action: " << action << endl;
        logFile.close();
    }
}

// Function to display menu
void showMenu() {
    cout << "\n==================================================\n";
    cout << "        CryptoLabX - Cryptanalysis Toolkit\n";
    cout << "==================================================\n";
    cout << "1. Encrypt\n";
    cout << "2. Decrypt\n";
    cout << "3. Attack\n";
    cout << "4. Analyze\n";
    cout << "5. Exit\n";
    cout << "==================================================\n";
}

// Function to analyze a file
void analyzeFile(string filename) {
    ifstream file("datasets/" + filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file!\n";
        return;
    }

    string content;
    string line;
    while (getline(file, line)) {
        content += line + "\n";
    }
    file.close();

    // Count characters, words, lines
    int numChars = content.length();
    int numWords = 0;
    int numLines = 0;
    int letterCount[26] = {0};

    // Count words
    for (int i = 0; i < numChars; i++) {
        if (content[i] == ' ' || content[i] == '\n') {
            numWords++;
        }
        if (content[i] == '\n') {
            numLines++;
        }
        // Count letters
        char c = content[i];
        if (c >= 'a' && c <= 'z') {
            letterCount[c - 'a']++;
        } else if (c >= 'A' && c <= 'Z') {
            letterCount[c - 'A']++;
        }
    }
    if (numChars > 0) numWords++;
    if (numLines == 0) numLines = 1;

    // Display results
    cout << "\n============================================================\n";
    cout << "File Analysis: " << filename << endl;
    cout << "============================================================\n";
    cout << "Total Characters: " << numChars << endl;
    cout << "Total Words: " << numWords << endl;
    cout << "Total Lines: " << numLines << endl;

    cout << "\nLetter Frequency:\n";
    cout << "------------------------------------------------------------\n";
    for (int i = 0; i < 26; i++) {
        if (letterCount[i] > 0) {
            cout << (char)('A' + i) << ": " << letterCount[i] << "  ";
            if ((i + 1) % 5 == 0) cout << endl;
        }
    }
    cout << "\n============================================================\n";
}

int main() {
    cout << "Welcome to CryptoLabX!\n";

    int choice;
    bool running = true;

    while (running) {
        showMenu();
        cout << "\nEnter your choice (1-5): ";
        cin >> choice;

        if (choice == 1) {
            cout << "\n[Encrypt] Coming Soon...\n";
            writeLog("Encrypt");
        }
        else if (choice == 2) {
            cout << "\n[Decrypt] Coming Soon...\n";
            writeLog("Decrypt");
        }
        else if (choice == 3) {
            cout << "\n[Attack] Coming Soon...\n";
            writeLog("Attack");
        }
        else if (choice == 4) {
            cout << "\n[Analyze] File Analysis\n";
            writeLog("Analyze");
            
            cout << "\nAvailable files:\n";
            cout << "1. sample1.txt\n";
            cout << "2. sample2.txt\n";
            cout << "3. sample3.txt\n";
            cout << "4. sample4.txt\n";
            cout << "5. sample5.txt\n";
            
            int fileChoice;
            cout << "\nSelect a file number: ";
            cin >> fileChoice;
            
            if (fileChoice >= 1 && fileChoice <= 5) {
                string filename = "sample" + to_string(fileChoice) + ".txt";
                analyzeFile(filename);
            } else {
                cout << "Invalid selection!\n";
            }
        }
        else if (choice == 5) {
            cout << "\nThank you for using CryptoLabX!\n";
            writeLog("Exit");
            running = false;
        }
        else {
            cout << "\nInvalid choice! Please select 1-5.\n";
        }
    }

    return 0;
}
