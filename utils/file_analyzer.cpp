/**
 * File analyzer implementation
 */

#include "file_analyzer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <set>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dirent.h>
#endif

FileAnalyzer::FileAnalyzer() {}

FileAnalyzer::~FileAnalyzer() {}

std::vector<std::string> FileAnalyzer::listDatasetFiles() {
    std::vector<std::string> files;
    std::string datasetsPath = "datasets";

#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((datasetsPath + "\\*.txt").c_str(), &findFileData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            files.push_back(findFileData.cFileName);
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
#else
    DIR* dir = opendir(datasetsPath.c_str());
    if (dir != nullptr) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string fileName = entry->d_name;
            if (fileName.length() > 4 && fileName.substr(fileName.length() - 4) == ".txt") {
                files.push_back(fileName);
            }
        }
        closedir(dir);
    }
#endif

    std::sort(files.begin(), files.end());
    return files;
}

void FileAnalyzer::analyzeFile(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cout << "Error: File '" << filePath << "' not found!" << std::endl;
            return;
        }

        // Read entire file content
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();

        // Calculate statistics
        FileStats stats = calculateStats(content);

        // Extract filename from path
        size_t lastSlash = filePath.find_last_of("/\\");
        std::string fileName = (lastSlash != std::string::npos) ? 
                               filePath.substr(lastSlash + 1) : filePath;

        // Display results
        displayStats(fileName, stats);

    } catch (const std::exception& e) {
        std::cout << "Error analyzing file: " << e.what() << std::endl;
    }
}

FileAnalyzer::FileStats FileAnalyzer::calculateStats(const std::string& content) {
    FileStats stats = {0, 0, 0, 0, {}};

    // Character count
    stats.numCharacters = content.length();

    // Line count
    stats.numLines = 0;
    for (char c : content) {
        if (c == '\n') stats.numLines++;
    }
    if (!content.empty() && content.back() != '\n') {
        stats.numLines++;
    }

    // Word count
    std::istringstream iss(content);
    std::string word;
    while (iss >> word) {
        stats.numWords++;
    }

    // Unique characters
    std::set<char> uniqueChars(content.begin(), content.end());
    stats.numUniqueChars = uniqueChars.size();

    // Letter frequency (only alphabetic characters)
    for (char c : content) {
        if (std::isalpha(c)) {
            char upperC = std::toupper(c);
            stats.letterFrequency[upperC]++;
        }
    }

    return stats;
}

void FileAnalyzer::displayStats(const std::string& fileName, const FileStats& stats) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "File Analysis: " << fileName << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "Total Characters: " << stats.numCharacters << std::endl;
    std::cout << "Total Words: " << stats.numWords << std::endl;
    std::cout << "Total Lines: " << stats.numLines << std::endl;
    std::cout << "Unique Characters: " << stats.numUniqueChars << std::endl;

    if (!stats.letterFrequency.empty()) {
        std::cout << "\nLetter Frequency Distribution:" << std::endl;
        std::cout << std::string(60, '-') << std::endl;

        // Convert map to vector for sorting by frequency
        std::vector<std::pair<char, int>> sortedFreq(
            stats.letterFrequency.begin(), 
            stats.letterFrequency.end()
        );

        // Sort by frequency (descending)
        std::sort(sortedFreq.begin(), sortedFreq.end(),
                  [](const std::pair<char, int>& a, const std::pair<char, int>& b) {
                      return a.second > b.second;
                  });

        // Display in columns (3 per row)
        for (size_t i = 0; i < sortedFreq.size(); i += 3) {
            for (size_t j = i; j < std::min(i + 3, sortedFreq.size()); j++) {
                std::cout << sortedFreq[j].first << ": " 
                          << std::setw(4) << sortedFreq[j].second << "  ";
            }
            std::cout << std::endl;
        }

        // Calculate total letters for percentages
        int totalLetters = 0;
        for (const auto& pair : stats.letterFrequency) {
            totalLetters += pair.second;
        }

        if (totalLetters > 0) {
            std::cout << "\nTop 5 Most Frequent Letters:" << std::endl;
            std::cout << std::string(60, '-') << std::endl;

            for (size_t i = 0; i < std::min(size_t(5), sortedFreq.size()); i++) {
                double percentage = (sortedFreq[i].second * 100.0) / totalLetters;
                std::cout << sortedFreq[i].first << ": " 
                          << std::setw(4) << sortedFreq[i].second << " ("
                          << std::fixed << std::setprecision(2) << std::setw(5)
                          << percentage << "%)" << std::endl;
            }
        }
    }

    std::cout << std::string(60, '=') << std::endl;
}
