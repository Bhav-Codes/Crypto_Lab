/**
 * File analysis utility for text files
 */

#ifndef FILE_ANALYZER_H
#define FILE_ANALYZER_H

#include <string>
#include <vector>
#include <map>

class FileAnalyzer {
public:
    FileAnalyzer();
    ~FileAnalyzer();

    void analyzeFile(const std::string& filePath);
    std::vector<std::string> listDatasetFiles();

private:
    struct FileStats {
        int numCharacters;
        int numWords;
        int numLines;
        int numUniqueChars;
        std::map<char, int> letterFrequency;
    };

    FileStats calculateStats(const std::string& content);
    void displayStats(const std::string& fileName, const FileStats& stats);
};

#endif // FILE_ANALYZER_H
