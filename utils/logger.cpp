/**
 * Logger implementation
 */

#include "logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

Logger::Logger(const std::string& dir) : logDir(dir), logFile(dir + "/activity.log") {
    // Create directory if it doesn't exist
    #ifdef _WIN32
        _mkdir(logDir.c_str());
    #else
        mkdir(logDir.c_str(), 0755);
    #endif

    initializeLogFile();
}

Logger::~Logger() {}

void Logger::initializeLogFile() {
    std::ifstream checkFile(logFile);
    if (!checkFile.good()) {
        // File doesn't exist, create it with header
        std::ofstream outFile(logFile);
        if (outFile.is_open()) {
            outFile << "CryptoLabX Activity Log" << std::endl;
            outFile << std::string(70, '=') << std::endl;
            outFile.close();
        }
    }
    checkFile.close();
}

void Logger::log(const std::string& action) {
    try {
        std::ofstream logFileStream(logFile, std::ios::app);
        if (!logFileStream.is_open()) {
            std::cerr << "Warning: Could not write to log file" << std::endl;
            return;
        }

        // Get current timestamp
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);

        // Format: [YYYY-MM-DD HH:MM:SS] Action: <action>
        logFileStream << "[" 
                      << std::setfill('0') << std::setw(4) << (localTime->tm_year + 1900) << "-"
                      << std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1) << "-"
                      << std::setfill('0') << std::setw(2) << localTime->tm_mday << " "
                      << std::setfill('0') << std::setw(2) << localTime->tm_hour << ":"
                      << std::setfill('0') << std::setw(2) << localTime->tm_min << ":"
                      << std::setfill('0') << std::setw(2) << localTime->tm_sec
                      << "] Action: " << action << std::endl;

        logFileStream.close();
    } catch (const std::exception& e) {
        std::cerr << "Warning: Could not write to log file: " << e.what() << std::endl;
    }
}

std::string Logger::getLogPath() const {
    return logFile;
}
